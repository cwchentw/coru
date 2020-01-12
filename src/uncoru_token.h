#ifndef UNCORU_TOKEN_H
#define UNCORU_TOKEN_H

typedef unsigned char UNCORU_TOKEN_TYPE;

#define  UNCORU_TOKEN_CODE          0  /* Any non-specified code. */
#define  UNCORU_TOKEN_SPACE         1  /* ' ' */
#define  UNCORU_TOKEN_TAB           2  /* '\t' */
#define  UNCORU_TOKEN_SINGLE_QUOTE  3  /* '\'' */
#define  UNCORU_TOKEN_DOUBLE_QUOTE  4  /* '"'  */
#define  UNCORU_TOKEN_BACKSLASH     5  /* '\\' */

typedef struct uncoru_token_t uncoru_token_t;

uncoru_token_t * uncoru_token_new(UNCORU_TOKEN_TYPE type, char *text);
void uncoru_token_delete(void *self);

#endif  /* UNCORU_TOKEN_H */