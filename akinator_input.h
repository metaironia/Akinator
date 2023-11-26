#ifndef AKINATOR_INPUT_H
#define AKINATOL_INPUT_H

#include "akinator_func.h"

enum UserAnswer AskUser (void);

enum UserAnswer AskUserLastNode (TreeNode *akinator_tree_node_for_last_ask,
                                 const char *last_node_object_name);

enum StringFuncStatus ScanUserString (char *array_for_scan, const size_t size_array_for_scan);

enum StringFuncStatus CheckIfStringTooLong (const char *array_for_string_check,
                                            const size_t size_array_for_string_check);

enum StringFuncStatus RemoveSpacesFromStringEnd (char *array_for_remove_end_spaces,
                                                 const size_t size_array_for_space_remove);

enum AkinatorFuncStatus CommandLineArgChecker (const int argcc, const char *argvv[]);

const char *AkinatorFileDatabaseName (const char *argvv[]);

#endif
