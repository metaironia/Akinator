#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree_func.h"
#include "tree_log.h"


enum TreeFuncStatus TreeCtor (Tree *tree_to_create) {

    assert (tree_to_create);

    (tree_to_create -> root) = CreateTreeNode ();

    return TREE_STATUS_OK;
}

TreeNode *CreateTreeNode (void) {

    TreeNode *tree_node_ptr = (TreeNode *) calloc (1, sizeof (TreeNode));

    assert (tree_node_ptr);

    return tree_node_ptr;
}

enum TreeFuncStatus TreeNodeCreateLeftBranch (TreeNode *node_for_add_left_branch) {

    assert (node_for_add_left_branch);

    (node_for_add_left_branch -> left_branch) = CreateTreeNode ();

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeNodeCreateRightBranch (TreeNode *node_for_add_right_branch) {

    assert (node_for_add_right_branch);

    (node_for_add_right_branch -> right_branch) = CreateTreeNode ();

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeReadFromFile (FILE *file_with_tree, Tree *tree_for_fill) {

    assert (file_with_tree);
    assert (tree_for_fill);

    if (TreeNodeRead (file_with_tree, &(tree_for_fill -> root)) == TREE_STATUS_FAIL) {

        fprintf (stderr, "Database is incorrect.");

        return TREE_STATUS_FAIL;
    }

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeNodeRead (FILE *file_for_read_tree, TreeNode **tree_node_for_fill) {  //PREORDER

    assert (file_for_read_tree);

    char *buf = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (buf);

    //HOW TO READ VARIABLE NUM OF SYMBOLS???

    if (IsBracketInFileStr (file_for_read_tree, '(') == false) {

        if (TreeNodeNilCheck (file_for_read_tree, buf) == TREE_STATUS_OK)
            return TREE_STATUS_OK;
        else
            return TREE_STATUS_FAIL;
    }


    ON_DEBUG (printf ("( "));

    *tree_node_for_fill = CreateTreeNode ();

    if (TreeNodeDataRead (file_for_read_tree, *tree_node_for_fill, buf) == TREE_STATUS_FAIL)
        return TREE_STATUS_FAIL;

    //recursion below

    if (TreeNodeRead (file_for_read_tree, &((*tree_node_for_fill) -> left_branch)) == TREE_STATUS_FAIL)
        return TREE_STATUS_FAIL;

    if (TreeNodeRead (file_for_read_tree, &((*tree_node_for_fill) -> right_branch)) == TREE_STATUS_FAIL)
        return TREE_STATUS_FAIL;

    //ON_DEBUG (printf ("|read two nodes|"));

    if (IsBracketInFileStr (file_for_read_tree, ')')) {

        ON_DEBUG (printf (") "));

        return TREE_STATUS_OK;
    }

    return TREE_STATUS_FAIL;
}

enum TreeFuncStatus TreeNodeNilCheck (FILE *file_for_node_nil_check, char *buffer_for_node_check) {

    assert (file_for_node_nil_check);
    assert (buffer_for_node_check);

    fscanf (file_for_node_nil_check, "%4s", buffer_for_node_check);

    if (strcmp (buffer_for_node_check, NIL) == 0) {

        ON_DEBUG (printf ("nil "));

        return TREE_STATUS_OK;
    }

    ON_DEBUG (printf ("wtf"));

    return TREE_STATUS_FAIL;
}

enum TreeFuncStatus TreeNodeDataRead (FILE *file_for_read_node_data, TreeNode *tree_node_for_data_read,
                                      char *buffer_for_read_node_data) {

    assert (file_for_read_node_data);
    assert (tree_node_for_data_read);
    assert (buffer_for_read_node_data);

    TreeElem_t *tree_node_data = &(tree_node_for_data_read -> data);

    if (IS_TREE_ELEM_PTR) {

        if (fscanf (file_for_read_node_data, " \" %100[^\"]", buffer_for_read_node_data)) {  //TODO fix num of read symbols

            #if IS_TREE_ELEM_PTR
                *tree_node_data = buffer_for_read_node_data;

                fseek (file_for_read_node_data, 1, SEEK_CUR);

            #endif

            ON_DEBUG (printf("data "));

            return TREE_STATUS_OK;
        }
    }

    else
        if (fscanf (file_for_read_node_data, TREE_DATA_FORMAT, tree_node_data)) {

            ON_DEBUG (printf ("data "));

            return TREE_STATUS_OK;
        }

    ON_DEBUG (printf ("wtf "));

    return TREE_STATUS_FAIL;
}

enum TreeFuncStatus TreeOutputToFile (FILE *file_for_output_tree, const Tree *tree_for_output) {

    assert (tree_for_output);
    assert (file_for_output_tree);

    TreeNodeOutputToFile (file_for_output_tree, tree_for_output -> root);

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeNodeOutputToFile (FILE *file_for_output_node,
                                          const TreeNode *tree_node_for_output) {

    assert (file_for_output_node);

    if (tree_node_for_output == NULL) {

        fprintf (file_for_output_node, "nil ");

        return TREE_STATUS_OK;
    }

    fprintf (file_for_output_node, "( ");

    fprintf (file_for_output_node, TREE_DATA_FORMAT " ", tree_node_for_output -> data);

    TreeNodeOutputToFile (file_for_output_node, tree_node_for_output -> left_branch);
    TreeNodeOutputToFile (file_for_output_node, tree_node_for_output -> right_branch);

    fprintf (file_for_output_node, ") ");

    return TREE_STATUS_OK;
}

unsigned int TreeVerify (const Tree *tree_for_verify) {      //TODO fix copypaste in verifier

    unsigned int errors_in_tree = 0;

    if (tree_for_verify == NULL) {

        errors_in_tree |= TREE_NULL_PTR;
        LogPrintTreeError ("TREE_NULL_PTR");

        return errors_in_tree;
    }

    TreeNode *root_node = tree_for_verify -> root;

    if (root_node == NULL) {

        errors_in_tree |= TREE_ROOT_NULL_PTR;
        LogPrintTreeError ("TREE_ROOT_NULL_PTR");

        return errors_in_tree;
    }

    if (TreeCycledNodeSearch (root_node) == TREE_STATUS_FAIL) {

        errors_in_tree |= TREE_CYCLED_NODE;
        LogPrintTreeError ("TREE_CYCLED_NODE");

        return errors_in_tree;
    }

    if (TreeNodeFromPoisonSearch (root_node) == TREE_STATUS_FAIL) {

        errors_in_tree |= BRANCH_FROM_POISON;
        LogPrintTreeError ("BRANCH_FROM_POISON");
    }

    return errors_in_tree;
}

enum TreeFuncStatus TreeCycledNodeSearch (const TreeNode *tree_node_for_cycle_search) {

    if (tree_node_for_cycle_search == NULL)
        return TREE_STATUS_OK;

    if (tree_node_for_cycle_search == tree_node_for_cycle_search -> left_branch ||
        tree_node_for_cycle_search == tree_node_for_cycle_search -> right_branch)

        return TREE_STATUS_FAIL;

    //recursion below

    if (TreeCycledNodeSearch (tree_node_for_cycle_search -> left_branch)  == TREE_STATUS_FAIL ||
        TreeCycledNodeSearch (tree_node_for_cycle_search -> right_branch) == TREE_STATUS_FAIL)

        return TREE_STATUS_FAIL;

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeNodeFromPoisonSearch (const TreeNode *tree_node_for_poison_search) {

    if (tree_node_for_poison_search == NULL)
        return TREE_STATUS_OK;

    if (IS_TREE_ELEM_POISON (tree_node_for_poison_search -> data) &&
        (tree_node_for_poison_search -> left_branch != NULL ||
        tree_node_for_poison_search -> right_branch != NULL))

        return TREE_STATUS_FAIL;

    //recursion below

    if (TreeNodeFromPoisonSearch (tree_node_for_poison_search -> left_branch)  == TREE_STATUS_FAIL ||
        TreeNodeFromPoisonSearch (tree_node_for_poison_search -> right_branch) == TREE_STATUS_FAIL)

        return TREE_STATUS_FAIL;

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeNodeDestruct (TreeNode **tree_node_for_destruct) {

    assert (tree_node_for_destruct);

    if (*tree_node_for_destruct == NULL)
        return TREE_STATUS_OK;

    TreeNodeDestruct (&((*tree_node_for_destruct) -> left_branch));
    TreeNodeDestruct (&((*tree_node_for_destruct) -> right_branch));

    memset (&((*tree_node_for_destruct) -> data), 0, sizeof (TreeElem_t));

    free (*tree_node_for_destruct);
    *tree_node_for_destruct = NULL;

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeDestruct (Tree *tree_for_destruct) {

    assert (tree_for_destruct);

    TreeNodeDestruct (&(tree_for_destruct -> root));

    return TREE_STATUS_OK;
}

bool IsBracketInFileStr (FILE *file_to_check_str, const char bracket_type) {

    assert (file_to_check_str);

    char scanned_bracket = '\0';

    fscanf (file_to_check_str, " %c", &scanned_bracket);

    if (scanned_bracket == bracket_type)
        return true;

    fseek (file_to_check_str, -1, SEEK_CUR);
    return false;
}

