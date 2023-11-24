#ifndef TREE_FUNC_H
#define TREE_FUNC_H


#define NIL                     "nil"

#ifndef TREE_DATA_FORMAT
    #define TREE_DATA_FORMAT    "%s"
#endif

#define DEBUG                   0

#if DEBUG

    #define ON_DEBUG(...)       {__VA_ARGS__;}
#else

    #define ON_DEBUG(...)
#endif

#define TREE_VERIFY(tree)   {                                        \
                                if (TreeVerify (tree) != 0) {        \
                                                                     \
                                    TreeGraphDump (tree);            \
                                                                     \
                                    return TREE_STATUS_FAIL;         \
                                }                                    \
                            }

#define TREE_NODE_VERIFY(tree_node)                                  \
                            {                                        \
                                if (TreeNodeVerify (tree_node) != 0) \
                                    return TREE_STATUS_FAIL;         \
                            }



typedef char* TreeElem_t;

#define IS_TREE_ELEM_STRING 1

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

enum TreeErrors {

    TREE_NULL_PTR,
    TREE_NODE_NULL_PTR,
    TREE_CYCLED_NODE,
    BRANCH_FROM_POISON
};

enum TreeFuncStatus TreeCtor (Tree *tree_to_create);

TreeNode *CreateTreeNode (void);

enum TreeFuncStatus TreeNodeLeftBranchCreate (TreeNode *node_for_add_left_branch);

enum TreeFuncStatus TreeNodeRightBranchCreate (TreeNode *node_for_add_right_branch);

enum TreeFuncStatus TreeReadFromFile (FILE *file_with_tree, Tree *tree_for_fill);

enum TreeFuncStatus TreeNodeRead (FILE *file_for_read_tree, TreeNode **tree_node_for_fill);

enum TreeFuncStatus TreeNodeNilCheck (FILE *file_for_node_nil_check, char *buffer_for_node_check);

enum TreeFuncStatus TreeNodeDataRead (FILE *file_for_read_node_data, TreeNode *tree_node_for_data_read,
                                      char *buffer_for_read_node_data);

enum TreeFuncStatus TreeOutputToFile (FILE *file_for_output_tree, const Tree *tree_for_output);

enum TreeFuncStatus TreeNodeOutputToFile (FILE *file_for_output_node,
                                          const TreeNode *tree_node_for_output);

enum TreeFuncStatus TreeCycledNodeSearch (const TreeNode *tree_node_for_cycle_search);

enum TreeFuncStatus TreeNodeFromPoisonSearch (const TreeNode *tree_node_for_poison_search);

enum TreeFuncStatus TreeNodeDestruct (TreeNode **tree_node_for_destruct);

enum TreeFuncStatus TreeDestruct (Tree *tree_for_destruct);

bool IsBracketInFileStr (FILE *file_to_check_str, const char bracket_type);

unsigned int TreeVerify (const Tree *tree_for_verify);

unsigned int TreeNodeVerify (const TreeNode *tree_node_for_verify);


#endif
