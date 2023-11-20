#include <assert.h>
#include <stdlib.h>

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

    ReadTreeNode (file_with_tree, tree_for_fill);



    return TREE_STATUS_OK;
}

enum TreeFuncStatus ReadTreeNode (FILE *file_for_read_tree, TreeNode *tree_node_for_fill) {  //PREORDER

    assert (file_for_read_tree);
    assert (tree_node_for_fill);

    char *buf = (char *) calloc (sizeof (char), NODE_READ_BUF_SIZE);

    //HOW TO READ VARIABLE NUM OF SYMBOLS???

    int value_to_add = POISON;
    int is_value     = fscanf (file_for_read_tree, "%d", &value);

    if (is_value)
        tree_node_for_fill -> value = value_to_add;

    fscanf (file_for_read_tree, "%5s", buf);

    if (IsBracketInFileStr (file_for_read_tree, '(')) {

        TreeNodeCreateLeftBranch (tree_node_for_fill);

        if (ReadTreeNode (file_for_read_tree, tree_node_for_fill -> left_branch) == TREE_STATUS_FAIL)
            return TREE_STATUS_FAIL;
    }

    else if (strcmp (buf, NIL) == 0)
        tree_node_for_fill -> left_branch = NULL;
    else
        return TREE_STATUS_FAIL;

    fscanf (file_for_read_tree, "%5s", buf);

    if (IsBracketInFileStr (file_for_read_tree, '(')) {

        TreeNodeCreateRightBranch (tree_node_for_fill)
        if (ReadTreeNode (file_for_read_tree, tree_node_for_fill -> right_branch) == TREE_STATUS_FAIL)
            return TREE_STATUS_FAIL;
    }

    else if (strcmp (buf, NIL) == 0)
        tree_node_for_fill -> left_branch = NULL;
    else
        return TREE_STATUS_FAIL;

    if (IsBracketInFileStr (file_for_read_tree, ')')
        return TREE_STATUS_OK;

    return TREE_STATUS_FAIL;
}

bool IsBracketInFileStr (FILE *file_to_check_str, char bracket) {

    assert (file_to_check_str);

    char scanned_bracket = '\0';
    char sym_after_bracket = '\0';

    int scanf_result = fscanf (file_to_check_str, " %c%c", &scanned_bracket, &sym_after_bracket);

    if (scanned_bracket == bracket && scanf_result = 2 &&
        (isspace (sym_after_bracket) || sym_after_bracket == '\0'))

        return true;

    return false;
}



