#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#ifdef _WIN32
    #include <windows.h>
#endif

/* Custom boolean type. */
#ifndef _WIN32
#ifdef __cplusplus
    #ifndef _BOOL_IS_DEFINED
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
        #define _BOOL_IS_DEFINED
    #endif  /* BOOL */
#else
    #if __STDC_VERSION__ < 199901L
        #ifndef _BOOL_IS_DEFINED
            typedef char BOOL;
            #define FALSE  0
            #define TRUE   1
            #define _BOOL_IS_DEFINED
        #endif  /* BOOL */
    #else
        #ifndef _BOOL_IS_DEFINED
            #include <stdbool.h>
            typedef bool BOOL;
            #define FALSE  false
            #define TRUE   true
            #define _BOOL_IS_DEFINED
        #endif  /* BOOL */
    #endif  /* C89 */
#endif  /* __cplusplus */
#endif

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

#endif  /* HASH_TABLE_H */
