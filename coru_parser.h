#ifndef CORU_PARSER_H
#define CORU_PARSER_H

typedef struct coru_parser_t coru_parser_t;

coru_parser_t * coru_parser_new(void);
void coru_parser_delete(void *self);

#endif  /* CORU_PARSER_H */
