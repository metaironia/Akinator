#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"

enum UserAnswer AskUser (char *array_for_ask, const size_t size_of_array_for_ask) {

    assert (array_for_ask);

    ScanUserString (array_for_ask, size_of_array_for_ask);

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

    for (int i = size_array_for_space_remove - 1; i >= 0; i--) {

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


