#include <stdio.h>

#include "tree_func.h"
#include "tree_log.h"
#include "akinator_func.h"

int main (int argc, const char *argv[]) {

    Tree akinator_tree = {};

    TreeCtor (&akinator_tree);

    if (CommandLineArgChecker (argc, argv) == AKINATOR_STATUS_FAIL)
        return -1;

    FILE *akinator_file_for_read = fopen (AkinatorFileDatabaseName (argv), "r");

    if (TreeReadFromFile (akinator_file_for_read, &akinator_tree) == TREE_STATUS_FAIL)
        return -2;

    fclose (akinator_file_for_read);
    akinator_file_for_read = NULL;

    TreeGraphDump (&akinator_tree);

    if (AkinatorGuess (&akinator_tree) == AKINATOR_STATUS_FAIL) {

        fprintf (stderr, "ERROR WHILE GUESSING\n");

        return -1;
    }

    while (AkinatorExit (AkinatorFileDatabaseName (argv), &akinator_tree) != AKINATOR_STATUS_OK)
        printf ("Enter YES or NO only.\n");

    return 0;
}
