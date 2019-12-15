#ifndef CORU_TOKEN_H
#define CORU_TOKEN_H

typedef char CORU_TOKEN_TYPE;

#define CORU_TOKEN_CODE           0  /* Any non-specified code. */
#define CORU_TOKEN_SPACE          1  /* ' ' */
#define CORU_TOKEN_TAB            2  /* '\t' */
#define CORU_TOKEN_SINGLE_QUOTE   3  /* '\'' */
#define CORU_TOKEN_DOUBLE_QUOTE   4  /* '"' */
#define CORU_TOKEN_BACKSLASH      5  /* '\\' */

typedef struct coru_token_t coru_token_t;

coru_token_t * coru_token_new(CORU_TOKEN_TYPE t, char *text);
CORU_TOKEN_TYPE coru_token_type(coru_token_t *self);
coru_token_t * coru_token_copy(coru_token_t *self);
void coru_token_delete(void *self);

#endif  /* CORU_TOKEN_H */
