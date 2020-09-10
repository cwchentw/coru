#ifndef UNCORU_PARSER_H
#define UNCORU_PARSER_H

#include "uncoru.h"
#include "uncoru_lexer.h"

typedef struct uncoru_parser_t uncoru_parser_t;

uncoru_parser_t * uncoru_parser_new(void);
BOOL uncoru_parser_parse(uncoru_parser_t *self, uncoru_lexer_t *lexer);
void uncoru_parser_delete(void *self);

#endif  /* CORU_PARSER_H */
