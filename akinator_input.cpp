#define TX_USE_SPEAK
#include "txlib/TXlib.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#include "tree/tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"

enum UserAnswer AskUser (void) {

    char user_answer_question[MAX_WORD_LENGTH] = {};

    ScanUserString (user_answer_question, MAX_WORD_LENGTH);

    if (strcmp (user_answer_question, "YES") == 0)
        return USER_ANSWER_YES;

    if (strcmp (user_answer_question, "NO") == 0)
        return USER_ANSWER_NO;

    return USER_ANSWER_ERROR;
}

enum UserAnswer AskUserLastNode (TreeNode *akinator_tree_node_for_last_ask) {

    assert (akinator_tree_node_for_last_ask);

    char *user_answer_person = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (user_answer_person);

    txSpeak ("\vWho is that?\n");

    ScanUserString (user_answer_person, NODE_READ_BUF_SIZE);

    AkinatorLastNodeSwap (akinator_tree_node_for_last_ask, user_answer_person);

    txSpeak ("\vWhat is the difference between them?\n");

    char *user_answer_question = (char *) calloc (NODE_READ_BUF_SIZE, sizeof (char));
    assert (user_answer_question);

    ScanUserString (user_answer_question, NODE_READ_BUF_SIZE);

    akinator_tree_node_for_last_ask -> data = user_answer_question;

    return USER_ANSWER_OK;
}

enum StringFuncStatus ScanUserString (char *array_for_scan, const size_t size_array_for_scan) {

    assert (array_for_scan);

    memset (array_for_scan, 0, size_array_for_scan * sizeof (array_for_scan[0]));

    scanf (" %[^\n]", array_for_scan);

    if (CheckIfStringTooLong (array_for_scan, size_array_for_scan) == STRING_STATUS_FAIL) {

        txSpeak ("\vYour string is too long, try once again.\n");

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

    for (int64_t i = size_array_for_space_remove - 1; i >= 0; i--) {

        if (array_for_remove_end_spaces[i] == '\0')
            continue;

        if (isspace (array_for_remove_end_spaces[i]))
            array_for_remove_end_spaces[i] = '\0';

        else
            break;
    }

    return STRING_STATUS_OK;
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


