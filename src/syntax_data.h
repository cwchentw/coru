#ifndef SYNTAX_H
#define SYNTAX_H

#include "hash_table.h"

hash_table_t * init_comment_multiple_start(void);
hash_table_t * init_comment_multiple_end(void);

const char * single_start_string(const char *key);
const char * single_end_string(const char *key);

#endif  /* SYNTAX_H */
