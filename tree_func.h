#ifndef TREE_FUNC_H
#define TREE_FUNC_H


#define NIL                     "nil"

#ifndef TREE_DATA_FORMAT
    #define TREE_DATA_FORMAT    "%s"
#endif

#define DEBUG                   1

#if DEBUG

    #define ON_DEBUG(...)       {__VA_ARGS__;}
#else

    #define ON_DEBUG(...)
#endif

typedef char* TreeElem_t;

const int POISON = 0xDEAD;

const int NODE_READ_BUF_SIZE = 100;

struct TreeNode {

    TreeElem_t data;

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

enum TreeFuncStatus TreeReadFromFile (FILE *file_with_tree, Tree *tree_for_fill);

enum TreeFuncStatus ReadTreeNode (FILE *file_for_read_tree, TreeNode **tree_node_for_fill);

bool IsBracketInFileStr (FILE *file_to_check_str, char bracket_type);

#endif
