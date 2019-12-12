#ifndef CORU_LEXER_H
#define CORU_LEXER_H

typedef struct coru_lexer_t coru_lexer_t;

coru_lexer_t * coru_lexer_new(char *input);
void coru_lexer_delete(void *self);

#endif  /* CORU_LEXER_H */
