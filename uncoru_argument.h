#ifndef UNCORU_ARGUMENT_H
#define UNCORU_ARGUMENT_H

typedef struct uncoru_argument_t uncoru_argument_t;

uncoru_argument_t * uncoru_argument_parse(int argc, char *argv[]);
void uncoru_argument_delete(void *self);

#endif  /* UNCORU_ARGUMENT_H */
