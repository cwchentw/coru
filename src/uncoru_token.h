#ifndef UNCORU_TOKEN_H
#define UNCORU_TOKEN_H

typedef unsigned char UNCORU_TOKEN_TYPE;

#define  UNCORU_TOKEN_CODE           0  /* Any non-specified code. */
#define  UNCORU_TOKEN_SPACE          1  /* ' ' */
#define  UNCORU_TOKEN_TAB            2  /* '\t' */
#define  UNCORU_TOKEN_SINGLE_QUOTE   3  /* '\'' */
#define  UNCORU_TOKEN_DOUBLE_QUOTE   4  /* '"'  */
#define  UNCORU_TOKEN_BACKSLASH      5  /* '\\' */
#define  UNCORU_TOKEN_AMPERSAND      6  /* '&' */
#define  UNCORU_TOKEN_COMMENT_START  7  /* The start text of comment. */
#define  UNCORU_TOKEN_COMMENT_END    8  /* The end text of comment. */
#define  UNCORU_TOKEN_INTEGER        9  /* Integer. */

typedef struct uncoru_token_t {
    UNCORU_TOKEN_TYPE token_t;
    char *text;
} uncoru_token_t;

uncoru_token_t * uncoru_token_new(UNCORU_TOKEN_TYPE type, char *text);
void uncoru_token_delete(void *self);
uncoru_token_t * uncoru_token_copy(uncoru_token_t *self);

#define uncoru_token_type(self) ((self)->token_t)
#define uncoru_token_text(self) ((self)->text)

#endif  /* UNCORU_TOKEN_H */
