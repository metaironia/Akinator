#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree_func.h"
#include "tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"


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

    akinator_node_for_swap -> data = POISON_NUM;

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

enum AkinatorFuncStatus AkinatorBegin (Tree *akinator_for_begin, const int akinator_argc,
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

//enum AkinatorFuncStatus AkinatorDescription (const Tree *akinator_tree_for_description) {
//
//    AKINATOR_TREE_VERIFY (akinator_tree_for_description)
//
//
//}
