#include <stdio.h>
#include <assert.h>

#include "tree_log.h"
#include "tree_func.h"

FILE *TREE_LOG_FILE = CreateLogFile ("tree_log.txt");

FILE *CreateLogFile (const char *log_file_name) {

    assert (log_file_name);

    FILE *log_file_ptr = fopen (log_file_name, "w");

    assert (log_file_ptr);

    atexit (CloseLogFile);

    return log_file_ptr;
}

void CloseLogFile (void) {

    if (TREE_LOG_FILE)
        fclose (TREE_LOG_FILE);

    LOG_FILE = NULL;
}

enum TreeFuncStatus LogPrintTreeError (const char *error_text) {

    assert (error_text);

    LOG_PRINT (TREE_LOG_FILE, "An error %s occurred. \n", error_text);

    return LIST_STATUS_OK;
}

enum TreeFuncStatus TreeGraphDump (Tree *tree_for_graph_dump) {

    assert (tree_for_graph_dump);

    FILE *tree_dot_file = fopen (TREE_DOT_FILE_NAME, "w");
    assert (tree_dot_file);

    TreeDotFileBegin (tree_dot_file);

    TreeDotFileCreateElements (tree_dot_file, tree_for_graph_dump -> root);
    TreeDotFileDrawArrows     (tree_dot_file, tree_for_graph_dump -> root);

    TreeDotFileEnd (tree_dot_file);

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeDotFileBegin (FILE *tree_dot_file_begin) {

    assert (tree_dot_file_begin);

    fprintf(tree_dot_file_begin, "digraph AkinatorTree{\n"
                                 "rankdir = TB;\n"
                                 "graph [bgcolor = white];\n");

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeDotFileEnd (FILE *tree_dot_file_end) {

    assert (tree_dot_file_end);

    fprintf(tree_dot_file_end, "\n}\n");

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeDotFileCreateElements (FILE *tree_dot_file_gen_elems,
                                               const TreeNode *tree_node_for_gen_elems) {

    assert (tree_dot_file_gen_elems);

    if (tree_node_for_gen_elems != NULL) {

        fprintf (tree_dot_file_gen_elems, NODE_START_SYMBOL "%x [shape=record, "
                                          "style = filled, fillcolor = lightgreen, "
                                          "fontsize = 16, fontname = \"times bold\", "
                                          "label=\"" TREE_DATA_FORMAT "\"];\n",
                                          (int) (tree_node_for_gen_elems),
                                          tree_node_for_gen_elems -> data);

        TreeDotFileCreateElements (tree_dot_file_gen_elems, tree_node_for_gen_elems -> left_branch);
        TreeDotFileCreateElements (tree_dot_file_gen_elems, tree_node_for_gen_elems -> right_branch);
    }

    return TREE_STATUS_OK;
}

enum TreeFuncStatus TreeDotFileDrawArrows (FILE *tree_dot_file_draw,
                                           const TreeNode *tree_node_for_draw_arrows) {

    assert (tree_dot_file_draw);
    assert (tree_node_for_draw_arrows);

    if (tree_node_for_draw_arrows -> left_branch != NULL) {

        fprintf (tree_dot_file_draw, NODE_START_SYMBOL "%x -> " NODE_START_SYMBOL "%x;\n",
                                     (int) (tree_node_for_draw_arrows),
                                     (int) (tree_node_for_draw_arrows -> left_branch));

        TreeDotFileDrawArrows (tree_dot_file_draw, tree_node_for_draw_arrows -> left_branch);
    }

    if (tree_node_for_draw_arrows -> right_branch != NULL) {

        fprintf (tree_dot_file_draw, NODE_START_SYMBOL "%x -> " NODE_START_SYMBOL "%x;\n",
                                     (int) (tree_node_for_draw_arrows),
                                     (int) (tree_node_for_draw_arrows -> right_branch));

        TreeDotFileDrawArrows (tree_dot_file_draw, tree_node_for_draw_arrows -> right_branch);
    }

    return TREE_STATUS_OK;
}
