#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stack/my_stack_func.h"

#include "tree/tree_func.h"
#include "tree/tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"


enum AkinatorFuncStatus AkinatorChooseMode (Tree *akinator_tree_for_mode_choose) {

    AKINATOR_TREE_VERIFY (akinator_tree_for_mode_choose);



    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorGuess (Tree *akinator_tree_database) {

    AKINATOR_TREE_VERIFY (akinator_tree_database);

    char *user_answer = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (user_answer);

    if (AkinatorNodeGuess (akinator_tree_database -> root, user_answer) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorNodeGuess (TreeNode *akinator_tree_node, char *array_for_answer) {

    AKINATOR_NODE_VERIFY (akinator_tree_node);

    printf ("Is that %s?\n", akinator_tree_node -> data);

    int user_ask_status = AskUser (array_for_answer, MAX_WORD_LENGTH);

    switch (user_ask_status) {

        case USER_ANSWER_YES:
            if (akinator_tree_node -> left_branch)
                AkinatorNodeGuess (akinator_tree_node -> left_branch, array_for_answer);

            else
                printf ("HAHAHA YOU ARE FOOL IT WAS REALLY EASY.\n");

            break;

        case USER_ANSWER_NO:
            if (akinator_tree_node -> right_branch)
                AkinatorNodeGuess (akinator_tree_node -> right_branch, array_for_answer);

            else
                AskUserLastNode (akinator_tree_node, array_for_answer);

            break;

        case USER_ANSWER_ERROR:
            printf ("Enter YES or NO only.\n");

            AkinatorNodeGuess (akinator_tree_node, array_for_answer);
            break;

        default:
            fprintf (stderr, "UNKNOWN ERROR\n");
            return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorLastNodeSwap (TreeNode *akinator_node_for_swap,
                                              char *user_answer_new_person) {

    AKINATOR_NODE_VERIFY (akinator_node_for_swap);

    assert (user_answer_new_person);

    TreeNodeLeftBranchCreate  (akinator_node_for_swap);
    TreeNodeRightBranchCreate (akinator_node_for_swap);

    akinator_node_for_swap -> left_branch -> data  = user_answer_new_person;
    akinator_node_for_swap -> right_branch -> data = akinator_node_for_swap -> data;

    akinator_node_for_swap -> data = TREE_POISON_NUM;

    return AKINATOR_STATUS_OK;
}

enum UserAnswer AkinatorContinue (void) {

    printf ("Do you want to continue?\n");

    char *array_ask_for_continue = (char *) calloc (MAX_WORD_LENGTH, sizeof (char));
    assert (array_ask_for_continue);

    UserAnswer user_answer_status = USER_ANSWER_ERROR;

    while ((user_answer_status = AskUser (array_ask_for_continue, MAX_WORD_LENGTH)) == USER_ANSWER_ERROR)
        printf ("Enter YES or NO only.\n");

    free (array_ask_for_continue);
    array_ask_for_continue = NULL;

    return user_answer_status;
}

enum AkinatorFuncStatus AkinatorReadDatabase (Tree *akinator_for_begin, const int akinator_argc,
                                              const char **akinator_argv) {

    if (CommandLineArgChecker (akinator_argc, akinator_argv) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    FILE *akinator_file_for_read = fopen (AkinatorFileDatabaseName (akinator_argv), "r");

    if (TreeReadFromFile (akinator_file_for_read, akinator_for_begin) == TREE_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    fclose (akinator_file_for_read);
    akinator_file_for_read = NULL;

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorDescription (const Tree *akinator_tree_for_description) {

    AKINATOR_TREE_VERIFY (akinator_tree_for_description)

    char *array_for_description_ask = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (array_for_description_ask);

    Stack stack_tree_path_to_element = {};
    StackCtor (&stack_tree_path_to_element, 1);

    if (AkinatorInputAndFindThePerson (akinator_tree_for_description, array_for_description_ask,
                                       &stack_tree_path_to_element) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    TreeNode *tree_node_current = akinator_tree_for_description -> root;

    printf ("%s is ", array_for_description_ask);

    TreeNextBranch next_branch = NODE_NO_BRANCH;

    AkinatorWholeDescriptionPrint (tree_node_current, next_branch, &stack_tree_path_to_element);

    printf ("that's all.\n");

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorOneDescriptionPrint (const TreeNode *tree_node_for_desc,
                                                     const TreeNextBranch node_next_branch) {

    AKINATOR_NODE_VERIFY (tree_node_for_desc);

    switch (node_next_branch) {

        case NODE_LEFT_BRANCH:
            printf ("%s, ", tree_node_for_desc -> data);
            break;

        case NODE_RIGHT_BRANCH:
            printf ("not %s, ", tree_node_for_desc -> data);
            break;

        case NODE_NO_BRANCH:
        default:
            fprintf (stderr, "SOMETHING WENT WRONG\n");
            return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorWholeDescriptionPrint (TreeNode *tree_node_for_desc,
                                                       TreeNextBranch node_next_branch,
                                                       Stack *stack_tree_path_to_person_print) {

    assert (stack_tree_path_to_person_print);

    AKINATOR_NODE_VERIFY (tree_node_for_desc);

    while ((stack_tree_path_to_person_print -> stack_size) >= 1) {

        node_next_branch = (TreeNextBranch) StackPop (stack_tree_path_to_person_print);

        if (AkinatorOneDescriptionPrint (tree_node_for_desc, node_next_branch) == AKINATOR_STATUS_FAIL)
            return AKINATOR_STATUS_FAIL;

        if (AkinatorBranchSwitch (&tree_node_for_desc, node_next_branch) == AKINATOR_STATUS_FAIL)
            return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorBranchSwitch (TreeNode **ptr_tree_node_for_switch,
                                              const TreeNextBranch node_next_branch) {

    assert (ptr_tree_node_for_switch);

    AKINATOR_NODE_VERIFY (*ptr_tree_node_for_switch);

    switch (node_next_branch) {

        case NODE_LEFT_BRANCH:
            *ptr_tree_node_for_switch = (*ptr_tree_node_for_switch) -> left_branch;
            break;

        case NODE_RIGHT_BRANCH:
            *ptr_tree_node_for_switch = (*ptr_tree_node_for_switch) -> right_branch;
            break;

        case NODE_NO_BRANCH:
        default:
            fprintf (stderr, "SOMETHING WENT WRONG\n");
            return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorInputAndFindThePerson (const Tree *tree_for_find_person,
                                                       char *array_for_person_name,
                                                       Stack *stack_for_path_to_person) {

    AKINATOR_TREE_VERIFY (tree_for_find_person);

    assert (array_for_person_name);
    assert (stack_for_path_to_person);

    printf ("What person do you have on the mind?\n");

    ScanUserString (array_for_person_name, NODE_READ_BUF_SIZE);

    if (TreeElementFind (tree_for_find_person,
                         array_for_person_name, stack_for_path_to_person) == TREE_STATUS_FAIL) {

        printf ("NO MATCHING RESULTS.\n");

        return AKINATOR_STATUS_FAIL;
    }

    printf ("%s naiden.\n", array_for_person_name);

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorDifference (const Tree *akinator_tree_for_diff) {

    AKINATOR_TREE_VERIFY (akinator_tree_for_diff);

    char *array_for_first_person_name = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (array_for_first_person_name);

    Stack stack_tree_path_first_person = {};
    StackCtor (&stack_tree_path_first_person, 1);

    if (AkinatorInputAndFindThePerson (akinator_tree_for_diff, array_for_first_person_name,
                                       &stack_tree_path_first_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    char *array_for_second_person_name = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (array_for_second_person_name);

    Stack stack_tree_path_second_person = {};
    StackCtor (&stack_tree_path_second_person, 1);

    if (AkinatorInputAndFindThePerson (akinator_tree_for_diff, array_for_second_person_name,
                                       &stack_tree_path_second_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    printf ("They are alike in that they are the ");

    TreeNextBranch first_person_next_branch  = NODE_NO_BRANCH;
    TreeNode *first_person_current_node      = akinator_tree_for_diff -> root;

    TreeNextBranch second_person_next_branch = NODE_NO_BRANCH;
    TreeNode *second_person_current_node     = akinator_tree_for_diff -> root;

    while ((stack_tree_path_first_person.stack_size >= 1)) {

        if ((first_person_next_branch  = (TreeNextBranch) StackPop (&stack_tree_path_first_person)) !=
            (second_person_next_branch = (TreeNextBranch) StackPop (&stack_tree_path_second_person))) {

            StackPush (&stack_tree_path_first_person, first_person_next_branch);
            StackPush (&stack_tree_path_second_person, second_person_next_branch);

            break;
        }

        if (AkinatorOneDescriptionPrint (first_person_current_node,
                                         first_person_next_branch) == AKINATOR_STATUS_FAIL)
            return AKINATOR_STATUS_FAIL;

        if (AkinatorBranchSwitch (&first_person_current_node,
                                  first_person_next_branch) == AKINATOR_STATUS_FAIL ||
            AkinatorBranchSwitch (&second_person_current_node,
                                  second_person_next_branch) == AKINATOR_STATUS_FAIL)

            return AKINATOR_STATUS_FAIL;
    }

    if (stack_tree_path_first_person.stack_size  == 0 ||
        stack_tree_path_second_person.stack_size == 0)

        return AKINATOR_STATUS_OK;

    printf ("but %s ", array_for_first_person_name);

    if (AkinatorWholeDescriptionPrint (first_person_current_node, first_person_next_branch,
                                       &stack_tree_path_first_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    printf ("and %s ", array_for_second_person_name);

    if (AkinatorWholeDescriptionPrint (second_person_current_node, second_person_next_branch,
                                       &stack_tree_path_second_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    printf ("and that's all.\n");

    return AKINATOR_STATUS_OK;
}
