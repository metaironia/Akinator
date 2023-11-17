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
