#ifndef UNCORU_PARSER_H
#define UNCORU_PARSER_H

#include "uncoru.h"

typedef struct uncoru_parser_t uncoru_parser_t;

uncoru_parser_t * uncoru_parser_new(void);
void uncoru_parser_delete(void *self);

#endif  /* CORU_PARSER_H */
