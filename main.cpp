#include <stdio.h>

#include "stack/my_stack_func.h"

#include "tree/tree_func.h"
#include "tree/tree_log.h"

#include "akinator_func.h"
#include "akinator_input.h"
#include "akinator_output.h"

int main (int argc, const char *argv[]) {

    Tree akinator_tree = {};
    TreeCtor (&akinator_tree);

    if (AkinatorReadDatabase (&akinator_tree, argc, argv) == AKINATOR_STATUS_FAIL)
        return -1;

    //TreeGraphDump (&akinator_tree);

    do {

        AkinatorChooseMode (&akinator_tree);

    } while (AkinatorContinue () == USER_ANSWER_YES);

    AkinatorExit (AkinatorFileDatabaseName (argv), &akinator_tree);

    return 0;
}
