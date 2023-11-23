#ifndef TREE_LOG_H
#define TREE_LOG_H

#include "tree_func.h"

#define TREE_DOT_FILE_NAME      "tree_dot_dump.dot"

#define NODE_START_SYMBOL       "A"

#ifndef TREE_DATA_FORMAT
    #define TREE_DATA_FORMAT    "%s"
#endif

#define LOG_PRINT(LOG_FILE, ...)  do {                                \
                                      fprintf(LOG_FILE, __VA_ARGS__); \
                                      fflush (LOG_FILE);              \
                                                                      \
                                  } while (0)

FILE *CreateLogFile (const char *log_file_name);

void CloseLogFile (void);

enum TreeFuncStatus LogPrintTreeError (const char *error_text);

enum TreeFuncStatus TreeGraphDump (Tree *tree_for_graph_dump);

enum TreeFuncStatus TreeDotFileBegin (FILE *tree_dot_file_begin);

enum TreeFuncStatus TreeDotFileEnd (FILE *tree_dot_file_end);

enum TreeFuncStatus TreeDotFileCreateElements (FILE *tree_dot_file_gen_elems,
                                               const TreeNode *tree_node_for_gen_elems);

enum TreeFuncStatus TreeDotFileDrawArrows (FILE *tree_dot_file_draw,
                                           const TreeNode *tree_node_for_draw_arrows);


#endif
