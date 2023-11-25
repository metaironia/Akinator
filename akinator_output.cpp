#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree/tree_func.h"
#include "tree/tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"
#include "akinator_output.h"

enum AkinatorFuncStatus AkinatorExit (const char *file_name_output,
                                      Tree *akinator_tree_at_exit) {

    AKINATOR_TREE_VERIFY (akinator_tree_at_exit);

    printf ("Do you want to save data?\n");

    char *user_answer = (char *) calloc (MAX_WORD_LENGTH, sizeof (char));
    assert (user_answer);

    ScanUserString (user_answer, MAX_WORD_LENGTH);

    if (strcmp (user_answer, "YES") == 0) {

        FILE *akinator_file_for_write = fopen (file_name_output, "w");

        TreeOutputToFile (akinator_file_for_write, akinator_tree_at_exit);

        fclose (akinator_file_for_write);
    }

    if (strcmp (user_answer, "NO") == 0);

    else
        return AKINATOR_STATUS_FAIL;

    free (user_answer);
    user_answer = NULL;

    TreeDestruct (akinator_tree_at_exit);

    return AKINATOR_STATUS_FAIL;
}
