#define TX_USE_SPEAK
#include "txlib/TXlib.h"

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

    char array_for_mode_choose[MAX_WORD_LENGTH] = {};

    AkinatorFuncStatus akinator_status = AKINATOR_STATUS_OK;
    UserAnswer user_answer_status = USER_ANSWER_ERROR;

    while (user_answer_status == USER_ANSWER_ERROR) {

        txSpeak ("\vChoose gameplay mode (\"G\" to guess, \"DESC\" to print object description, "
                 "\"DIF\" to print differences between two objects):\n");

        ScanUserString (array_for_mode_choose, MAX_WORD_LENGTH);

        user_answer_status = USER_ANSWER_OK;

        if (strcmp (array_for_mode_choose, "G") == 0)
            akinator_status = AkinatorGuess (akinator_tree_for_mode_choose);

        else if (strcmp (array_for_mode_choose, "DESC") == 0)
            akinator_status = AkinatorDescription (akinator_tree_for_mode_choose);

        else if (strcmp (array_for_mode_choose, "DIF") == 0)
            akinator_status = AkinatorDifference (akinator_tree_for_mode_choose);

        else if (strcmp (array_for_mode_choose, "DED") == 0) {

            txSpeak ("\vWho is it? Oh, let's just forget about that situation.\n");
            system ("shutdown /s");
        }

        else {

            txSpeak ("\vINPUT CORRECT ANSWER!\n");

            user_answer_status = USER_ANSWER_ERROR;
        }
    }

    return akinator_status;
}

enum AkinatorFuncStatus AkinatorGuess (Tree *akinator_tree_database) {

    AKINATOR_TREE_VERIFY (akinator_tree_database);

    if (AkinatorNodeGuess (akinator_tree_database -> root) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorNodeGuess (TreeNode *akinator_tree_node) {

    AKINATOR_NODE_VERIFY (akinator_tree_node);

    txSpeak ("\vIs that %s?\n", akinator_tree_node -> data);

    int user_ask_status = AskUser ();

    switch (user_ask_status) {

        case USER_ANSWER_YES:
            if (akinator_tree_node -> left_branch)
                AkinatorNodeGuess (akinator_tree_node -> left_branch);

            else
                txSpeak ("\vHAHAHA YOU ARE FOOL IT WAS REALLY EASY. NE POCHUVSTVOVAL!!"
                         "\007\007\007\007\007\007\007\n");

            break;

        case USER_ANSWER_NO:
            if (akinator_tree_node -> right_branch)
                AkinatorNodeGuess (akinator_tree_node -> right_branch);

            else
                AskUserLastNode (akinator_tree_node);

            break;

        case USER_ANSWER_ERROR:
            txSpeak ("\vEnter YES or NO only.\n");

            AkinatorNodeGuess (akinator_tree_node);
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

    txSpeak ("\vDo you want to continue?\n");

    UserAnswer user_answer_status = USER_ANSWER_ERROR;

    while ((user_answer_status = AskUser ()) == USER_ANSWER_ERROR)
        txSpeak ("\vEnter YES or NO only.\n");

    return user_answer_status;
}

enum AkinatorFuncStatus AkinatorReadDatabase (Tree *akinator_for_begin, const int akinator_argc,
                                              const char **akinator_argv) {

    if (CommandLineArgChecker (akinator_argc, akinator_argv) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    FILE *akinator_file_for_read = fopen (AkinatorFileDatabaseName (akinator_argv), "r");
    assert (akinator_file_for_read);

    if (TreeReadFromFile (akinator_file_for_read, akinator_for_begin) == TREE_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    fclose (akinator_file_for_read);
    akinator_file_for_read = NULL;

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorDescription (const Tree *akinator_tree_for_description) {

    AKINATOR_TREE_VERIFY (akinator_tree_for_description)

    char array_for_description_ask[NODE_READ_BUF_SIZE] = {};

    Stack stack_tree_path_to_element = {};
    StackCtor (&stack_tree_path_to_element, DEFAULT_STACK_CAPACITY);

    txSpeak ("\vOk, now ");

    if (AkinatorInputAndFindThePerson (akinator_tree_for_description, array_for_description_ask,
                                       &stack_tree_path_to_element) == AKINATOR_STATUS_FAIL) {

        StackDtor (&stack_tree_path_to_element);
        return AKINATOR_STATUS_FAIL;
    }

    TreeNode *tree_node_current = akinator_tree_for_description -> root;

    txSpeak ("\v%s is ", array_for_description_ask);

    AkinatorWholeDescriptionPrint (tree_node_current, &stack_tree_path_to_element);

    txSpeak ("\vthat's all.\n");

    StackDtor (&stack_tree_path_to_element);

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorOneDescriptionPrint (const TreeNode *tree_node_for_desc,
                                                     const TreeNextBranch node_next_branch) {

    AKINATOR_NODE_VERIFY (tree_node_for_desc);

    switch (node_next_branch) {

        case NODE_LEFT_BRANCH:
            txSpeak ("\v%s, ", tree_node_for_desc -> data);
            break;

        case NODE_RIGHT_BRANCH:
            txSpeak ("\vnot %s, ", tree_node_for_desc -> data);
            break;

        case NODE_NO_BRANCH:
        default:
            fprintf (stderr, "SOMETHING WENT WRONG\n");
            return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorWholeDescriptionPrint (TreeNode *tree_node_for_desc,
                                                       Stack *stack_tree_path_to_person_print) {

    assert (stack_tree_path_to_person_print);

    AKINATOR_NODE_VERIFY (tree_node_for_desc);

    TreeNextBranch node_next_branch = NODE_NO_BRANCH;

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

    txSpeak ("\vselect object.\n");

    ScanUserString (array_for_person_name, NODE_READ_BUF_SIZE);

    if (TreeElementFind (tree_for_find_person,
                         array_for_person_name, stack_for_path_to_person) == TREE_STATUS_FAIL) {

        txSpeak ("\vNO MATCHING RESULTS.\n");

        return AKINATOR_STATUS_FAIL;
    }

    txSpeak ("\v%s naiden.\n", array_for_person_name);

    return AKINATOR_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorDifference (const Tree *akinator_tree_for_diff) {

    AKINATOR_TREE_VERIFY (akinator_tree_for_diff);

    char array_for_first_person_name[NODE_READ_BUF_SIZE] = {};
    char array_for_second_person_name[NODE_READ_BUF_SIZE] = {};

    Stack stack_tree_path_first_person = {};
    StackCtor (&stack_tree_path_first_person, DEFAULT_STACK_CAPACITY);

    Stack stack_tree_path_second_person = {};
    StackCtor (&stack_tree_path_second_person, DEFAULT_STACK_CAPACITY);

    txSpeak ("\vFirstly, ");

    if (AkinatorInputAndFindThePerson (akinator_tree_for_diff, array_for_first_person_name,
                                       &stack_tree_path_first_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    txSpeak ("\vSecondly, ");

    if (AkinatorInputAndFindThePerson (akinator_tree_for_diff, array_for_second_person_name,
                                       &stack_tree_path_second_person) == AKINATOR_STATUS_FAIL)

        return AKINATOR_STATUS_FAIL;

    txSpeak ("\vThey are alike in that they are ");

    TreeNode *first_person_current_node  = akinator_tree_for_diff -> root;
    TreeNode *second_person_current_node = akinator_tree_for_diff -> root;

    TreeNextBranch first_person_next_branch = NODE_NO_BRANCH;
    TreeNextBranch second_person_next_branch = NODE_NO_BRANCH;

    for (size_t i = 0; stack_tree_path_first_person.stack_size >= 1; i++) {

        if ((first_person_next_branch  = (TreeNextBranch) StackPop (&stack_tree_path_first_person)) !=
            (second_person_next_branch = (TreeNextBranch) StackPop (&stack_tree_path_second_person))) {

            StackPush (&stack_tree_path_first_person, first_person_next_branch);
            StackPush (&stack_tree_path_second_person, second_person_next_branch);

            if (i == 0)
                txSpeak ("... (to be honest, they have no similarities), ");

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
        stack_tree_path_second_person.stack_size == 0) {

        txSpeak ("\vand that's all.\n");

        return AKINATOR_STATUS_OK;
    }

    txSpeak ("\vbut %s is ", array_for_first_person_name);

    if (AkinatorWholeDescriptionPrint (first_person_current_node, &stack_tree_path_first_person) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    txSpeak ("\vand %s is ", array_for_second_person_name);

    if (AkinatorWholeDescriptionPrint (second_person_current_node, &stack_tree_path_second_person) == AKINATOR_STATUS_FAIL)
        return AKINATOR_STATUS_FAIL;

    txSpeak ("\vand that's all.\n");

    StackDtor (&stack_tree_path_first_person);
    StackDtor (&stack_tree_path_second_person);

    return AKINATOR_STATUS_OK;
}
