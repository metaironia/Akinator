#ifndef AKINATOR_FUNC_H
#define AKINATOR_FUNC_H

#include "tree_func.h"

#define AKINATOR_TREE_VERIFY(tree)       {                                      \
                                             if (TreeVerify (tree) != 0) {      \
                                                                                \
                                                 TreeGraphDump (tree);          \
                                                                                \
                                                 return AKINATOR_STATUS_FAIL;   \
                                             }                                  \
                                         }

#define AKINATOR_NODE_VERIFY(tree_node)  {                                        \
                                             if (TreeNodeVerify (tree_node) != 0) \
                                                 return AKINATOR_STATUS_FAIL;     \
                                         }

enum AkinatorFuncStatus {

    AKINATOR_STATUS_OK,
    AKINATOR_STATUS_FAIL
};

enum StringFuncStatus {

    STRING_STATUS_OK,
    STRING_STATUS_FAIL
};

enum UserAnswer {

    USER_ANSWER_YES,
    USER_ANSWER_NO,
    USER_ANSWER_OK,
    USER_ANSWER_ERROR
};

enum AkinatorFuncStatus AkinatorGuess (Tree *akinator_tree_database);

enum AkinatorFuncStatus AkinatorNodeGuess (TreeNode *akinator_tree_node, char *array_for_answer);

enum UserAnswer AskUser (const TreeNode *akinator_tree_node_for_ask, char *array_for_ask);

enum UserAnswer AskUserLastNode (TreeNode *akinator_tree_node_for_last_ask,
                                 char *array_for_last_ask);

enum AkinatorFuncStatus AkinatorLastNodeSwap (TreeNode *akinator_node_for_swap,
                                              char *user_answer_new_person);

enum StringFuncStatus ScanUserString (char *array_for_scan, const size_t size_array_for_scan);

enum StringFuncStatus CheckIfStringTooLong (const char *array_for_string_check,
                                            const size_t size_array_for_string_check);

enum StringFuncStatus RemoveSpacesFromStringEnd (char *array_for_remove_end_spaces,
                                                 const size_t size_array_for_space_remove);


#endif
