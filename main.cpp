#include <stdio.h>

#include "tree_func.h"
#include "tree_log.h"

int main (void) {

    Tree test_tree = {};

    TreeCtor (&test_tree);

    FILE *test_file = fopen ("test.txt", "r");

    TreeReadFromFile (test_file, &test_tree);
printf ("tree address = %p, root = %p, left = %p, right = %p\n",
        &test_tree, test_tree.root, test_tree.root -> left_branch, test_tree.root -> right_branch);

    TreeGraphDump (&test_tree);

    return 0;
}
