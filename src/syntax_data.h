#ifndef SYNTAX_H
#define SYNTAX_H

#include "hash_table.h"

hash_table_t * init_comment_single_start(void);
hash_table_t * init_comment_single_end(void);
hash_table_t * init_comment_multiple_start(void);
hash_table_t * init_comment_multiple_end(void);

#endif  /* SYNTAX_H */
