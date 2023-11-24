#ifndef AKINATOR_FUNC_H
#define AKINATOR_FUNC_H

#include "tree/tree_func.h"

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

const int MAX_WORD_LENGTH = 10;

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

//enum AkinatorFuncStatus AkinatorChooseMode (Tree *akinator_tree_for_mode_choose);

enum AkinatorFuncStatus AkinatorReadDatabase (Tree *akinator_for_begin, const int akinator_argc,
                                              const char **akinator_argv);

enum AkinatorFuncStatus AkinatorGuess (Tree *akinator_tree_database);

enum AkinatorFuncStatus AkinatorNodeGuess (TreeNode *akinator_tree_node, char *array_for_answer);

enum AkinatorFuncStatus AkinatorLastNodeSwap (TreeNode *akinator_node_for_swap,
                                              char *user_answer_new_person);

enum AkinatorFuncStatus AkinatorDescription (const Tree *akinator_tree_for_description);

enum AkinatorFuncStatus AkinatorDifference (const Tree *akinator_tree_for_diff);

enum AkinatorFuncStatus AkinatorOneDescriptionPrint (const TreeNode *tree_node_for_desc,
                                                     const TreeNextBranch node_next_branch);

enum AkinatorFuncStatus AkinatorWholeDescriptionPrint (TreeNode *tree_node_for_desc,
                                                       TreeNextBranch node_next_branch,
                                                       Stack *stack_tree_path_to_person_print);

enum AkinatorFuncStatus AkinatorBranchSwitch (TreeNode **ptr_tree_node_for_switch,
                                              const TreeNextBranch node_next_branch);

enum AkinatorFuncStatus AkinatorInputAndFindThePerson (const Tree *tree_for_find_person,
                                                       char *array_for_person_name,
                                                       Stack *stack_for_path_to_person);

enum UserAnswer AkinatorContinue (void);


#endif
