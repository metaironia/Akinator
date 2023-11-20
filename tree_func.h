#ifndef TREE_FUNC_H
#define TREE_FUNC_H


#define NODE_READ_BUF_SIZE  5

#define NIL                 "nil"

typedef int TreeElem_t;

struct TreeNode {

    TreeElem_t value;

    TreeNode *left_branch;
    TreeNode *right_branch;
};

struct Tree {

    TreeNode *root;
};

enum TreeFuncStatus {

    TREE_STATUS_OK,
    TREE_STATUS_FAIL
};

enum TreeFuncStatus TreeCtor (Tree *tree_to_create);

TreeNode *CreateTreeNode (void);

enum TreeFuncStatus TreeNodeCreateLeftBranch (TreeNode *node_for_add_left_branch);

enum TreeFuncStatus TreeNodeCreateRightBranch (TreeNode *node_for_add_right_branch);


#endif
