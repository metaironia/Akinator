#include <stdio.h>

#include "tree_func.h"
#include "tree_log.h"
#include "akinator_func.h"

int main (void) {

    Tree akinator_tree = {};

    TreeCtor (&akinator_tree);

    FILE *akinator_file = fopen ("akinator_database.txt", "r");

    TreeReadFromFile (akinator_file, &akinator_tree);

    TreeGraphDump (&akinator_tree);

    if (AkinatorGuess (&akinator_tree) == AKINATOR_STATUS_FAIL) {

        fprintf (stderr, "ERROR WHILE GUESSING\n");

        return -1;
    }

    FILE *test_output_file = fopen ("test_output_file", "w");

    TreeOutputToFile (test_output_file, &akinator_tree);

    return 0;
}
