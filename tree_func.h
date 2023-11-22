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

/// 1 if tree element type is floating point number, 0 if not.
#define IS_TREE_ELEM_FLOAT  0

#define IS_TREE_ELEM_PTR    1

/// Defines how to check if tree element is poison number.
#if IS_TREE_ELEM_FLOAT

    const TreeElem_t POISON_NUM = NAN;                  ///< Poison number if tree element type is floating point.
    #define IS_TREE_ELEM_POISON(x)  isnan ((float) x)   ///< Method to check if tree element is poison.
#else

    #if IS_TREE_ELEM_PTR
        const TreeElem_t POISON_NUM = NULL;             ///< Poison number if tree element type is pointer.
    #else
        const TreeElem_t POISON_NUM = 0xDEAD;               ///< Poison number if tree element type is integer.
    #endif

    #define IS_TREE_ELEM_POISON(x)  x == POISON_NUM     ///< Method to check if tree element is poison.
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

bool IsBracketInFileStr (FILE *file_to_check_str, const char bracket_type);

#endif
