#ifndef CORU_TOKEN_H
#define CORU_TOKEN_H

typedef char CORU_TOKEN_TYPE;

#define CORU_TOKEN_CODE           0  /* Any non-specified code. */
#define CORU_TOKEN_SPACE          1  /* Space */
#define CORU_TOKEN_TAB            2  /* TAB */
#define CORU_TOKEN_SINGLE_QUOTE   3  /* ' */
#define CORU_TOKEN_DOUBLE_QUOTE   4  /* " */
#define CORU_TOKEN_TEXT           5  /* Text in quote. */
#define CORU_TOKEN_BACKSLASH      6  /* \ */

typedef struct coru_token_t coru_token_t;

coru_token_t * coru_token_new(CORU_TOKEN_TYPE t, char *text);
void coru_token_delete(void *self);

#endif  /* CORU_TOKEN_H */
