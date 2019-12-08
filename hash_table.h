#ifndef CORU_HASH_TABLE_H
#define CORU_HASH_TABLE_H

#include "boolean.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash_table_t hash_table_t;

hash_table_t * hash_table_new(void);
BOOL hash_table_add(hash_table_t *self, char *key, char *value);
char * hash_table_get(hash_table_t *self, char *key);
BOOL hash_table_remove(hash_table_t *self, char *key);
void hash_table_delete(void *self);

#ifdef __cplusplus
}
#endif

#endif  /* CORU_HASH_TABLE_H */
