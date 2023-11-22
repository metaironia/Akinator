#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree_func.h"


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

    if (ReadTreeNode (file_with_tree, &(tree_for_fill -> root)) == TREE_STATUS_FAIL) {

        fprintf (stderr, "Database is incorrect.");

        return TREE_STATUS_FAIL;
    }

    return TREE_STATUS_OK;
}

enum TreeFuncStatus ReadTreeNode (FILE *file_for_read_tree, TreeNode **tree_node_for_fill) {  //PREORDER

    assert (file_for_read_tree);
    assert (tree_node_for_fill);

    char *buf = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));

    assert (buf);

    //HOW TO READ VARIABLE NUM OF SYMBOLS???

    if (IsBracketInFileStr (file_for_read_tree, '(') == false) {

        fscanf (file_for_read_tree, "%4s", buf);

        if (strcmp (buf, NIL) == 0) {

            free (*tree_node_for_fill);
            *tree_node_for_fill = NULL;

            ON_DEBUG (printf ("nil "));

            return TREE_STATUS_OK;
        }

        ON_DEBUG (printf ("wtf"));

        return TREE_STATUS_FAIL;
    }

    ON_DEBUG (printf ("("));

    if (fscanf (file_for_read_tree, TREE_DATA_FORMAT, buf)) {
        (*tree_node_for_fill) -> data = buf;

    ON_DEBUG (printf ("data "));

    }

    else
        return TREE_STATUS_FAIL;

    TreeNodeCreateLeftBranch (*tree_node_for_fill);

    if (ReadTreeNode (file_for_read_tree, &((*tree_node_for_fill) -> left_branch)) == TREE_STATUS_FAIL)
        return TREE_STATUS_FAIL;


    TreeNodeCreateRightBranch (*tree_node_for_fill);

    if (ReadTreeNode (file_for_read_tree, &((*tree_node_for_fill) -> right_branch)) == TREE_STATUS_FAIL)
        return TREE_STATUS_FAIL;

    //ON_DEBUG (printf ("|read two nodes|"));

    if (IsBracketInFileStr (file_for_read_tree, ')')) {

    ON_DEBUG (printf (")"));

        return TREE_STATUS_OK;
    }

    return TREE_STATUS_FAIL;
}

unsigned int TreeVerify (Tree *tree_for_verify ) {      //TODO fix copypaste in verifier

    unsigned int errors_in_tree = 0;.

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
        LogPrintTreeError ("TREE_CYCLED_NODE")

        return errors_in_tree;
    }

    if (TreeNodeFromPoisonSearch (root_node) == TREE_STATUS_FAIL) {

        errors_in_tree |= BRANCH_FROM_POISON;
        LogPrintTreeError ("BRANCH_FROM_POISON");
    }

    return errors_in_tree;
}

enum TreeFuncStatus TreeCycledNodeSearch (TreeNode *tree_node_for_cycle_search) {

    if (tree_node_for_cycle_search == NULL)
        return TREE_STATUS_OK;

    if (tree_node_for_cycle_search == tree_node_for_cycle_search -> left_branch ||
        tree_node_for_cycle_search == tree_node_for_cycle_search -> right_branch)

        return TREE_STATUS_FAIL;

    TreeCycledNodeSearch (tree_node_for_cycle_search -> left_branch);
    TreeCycledNodeSearch (tree_node_for_cycle_search -> right_branch);
}

enum TreeFuncStatus TreeNodeFromPoisonSearch (TreeNode *tree_node_for_poison_search) {

    if (tree_node_for_poison_search == NULL)
        return TREE_STATUS_OK;

    if (IS_TREE_ELEM_POISON (tree_node_for_poison_search -> value) &&
        (tree_node_for_cycle_search -> left_branch != NULL ||
        tree_node_for_cycle_search -> right_branch != NULL))

        return TREE_STATUS_FAIL;

    TreeNodeFromPoisonSearch (tree_node_for_poison_search -> left_branch);
    TreeNodeFromPoisonSearch (tree_node_for_poison_search -> right_branch);
}

bool IsBracketInFileStr (FILE *file_to_check_str const char bracket_type) {

    assert (file_to_check_str);

    char scanned_bracket = '\0';

    fscanf (file_to_check_str, " %c", &scanned_bracket);

    if (scanned_bracket == bracket_type)
        return true;

    fseek (file_to_check_str, -1, SEEK_CUR);
    return false;
}



