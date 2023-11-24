#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "akinator_func.h"
#include "tree_func.h"
#include "tree_log.h"

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

    int user_ask_status = AskUser (akinator_tree_node, array_for_answer);

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

enum UserAnswer AskUser (const TreeNode *akinator_tree_node_for_ask, char *array_for_ask) {

    assert (akinator_tree_node_for_ask);

    printf ("Is that %s?\n", akinator_tree_node_for_ask -> data);

    ScanUserString (array_for_ask, NODE_READ_BUF_SIZE);

    if (strcmp (array_for_ask, "YES") == 0)
        return USER_ANSWER_YES;

    if (strcmp (array_for_ask, "NO") == 0)
        return USER_ANSWER_NO;

    return USER_ANSWER_ERROR;
}

enum UserAnswer AskUserLastNode (TreeNode *akinator_tree_node_for_last_ask,
                                 char *array_for_last_ask) {

    assert (akinator_tree_node_for_last_ask);

    printf ("Who is that?\n");

    ScanUserString (array_for_last_ask, NODE_READ_BUF_SIZE);

    AkinatorLastNodeSwap (akinator_tree_node_for_last_ask, array_for_last_ask);

    printf ("What is the difference between them?\n");

    char *array_new_question_node = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));

    ScanUserString (array_new_question_node, NODE_READ_BUF_SIZE);

    akinator_tree_node_for_last_ask -> data = array_new_question_node;

    return USER_ANSWER_OK;
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

enum StringFuncStatus ScanUserString (char *array_for_scan, const size_t size_array_for_scan) {

    assert (array_for_scan);

    memset (array_for_scan, 0, size_array_for_scan * sizeof (array_for_scan[0]));

    scanf (" %[^\n]", array_for_scan);

    if (CheckIfStringTooLong (array_for_scan, size_array_for_scan) == STRING_STATUS_FAIL) {

        printf ("Your string is too long, try once again.\n");

        return ScanUserString (array_for_scan, size_array_for_scan);
    }

    RemoveSpacesFromStringEnd (array_for_scan, size_array_for_scan);

    return STRING_STATUS_OK;
}

enum StringFuncStatus CheckIfStringTooLong (const char *array_for_string_check,
                                            const size_t size_array_for_string_check) {

    assert (array_for_string_check);
    assert (size_array_for_string_check > 0);

    if (array_for_string_check[size_array_for_string_check - 1] != '\0' &&
        !isspace (array_for_string_check[size_array_for_string_check - 1]))

        return STRING_STATUS_FAIL;

    return STRING_STATUS_OK;
}

enum StringFuncStatus RemoveSpacesFromStringEnd (char *array_for_remove_end_spaces,
                                                 const size_t size_array_for_space_remove) {

    assert (array_for_remove_end_spaces);
    assert (size_array_for_space_remove > 0);

    for (int i = size_array_for_space_remove - 1; i >= 0; i--)
        if (isspace (array_for_remove_end_spaces[i]))
            array_for_remove_end_spaces[i] = '\0';

    return STRING_STATUS_OK;
}

enum AkinatorFuncStatus AkinatorExit (const char *file_name_output,
                                      const Tree *akinator_tree_at_exit) {

    AKINATOR_TREE_VERIFY (akinator_tree_at_exit);

    printf ("Do you want to save new data?\n");

    char *user_answer = (char *) calloc (MAX_WORD_LENGTH, sizeof (char));

    ScanUserString (user_answer, MAX_WORD_LENGTH);

    if (strcmp (user_answer, "YES") == 0) {

        FILE *akinator_file_for_write = fopen (file_name_output, "w");

        TreeOutputToFile (akinator_file_for_write, akinator_tree_at_exit);

        fclose (akinator_file_for_write);

        return AKINATOR_STATUS_OK;
    }

    if (strcmp (user_answer, "NO") == 0)
        return AKINATOR_STATUS_OK;

    return AKINATOR_STATUS_FAIL;
}

enum AkinatorFuncStatus CommandLineArgChecker (const int argcc, const char *argvv[]) {

    assert (argvv);

    if (argcc < 2) {

        fprintf (stderr, "Not enough arguments.");
        return AKINATOR_STATUS_FAIL;
    }

    if (argcc > 2) {

        fprintf (stderr, "Too much arguments.");
        return AKINATOR_STATUS_FAIL;
    }

    return AKINATOR_STATUS_OK;
}

const char *AkinatorFileDatabaseName (const char *argvv[]) {

    return argvv[1];
}

