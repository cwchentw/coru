#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

typedef struct key_value_pair_t key_value_pair_t;

struct hash_table_t {
    key_value_pair_t **pairs;
    size_t size;
    size_t capacity;
};

struct key_value_pair_t {
    char *key;
    char *value;
    key_value_pair_t *next;
};

hash_table_t * hash_table_new(void)
{
    hash_table_t *table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (!table) {
        fprintf(stderr, "Failed to allocate memory for a hash table\n");
        fprintf(stderr, "Check available system memory\n");
        return table;
    }

    table->size = 0;
    table->capacity = 11;  /* Sensible initial capacity for a hash table. */
    table->pairs = \
        (key_value_pair_t **) \
        malloc(table->capacity * sizeof(key_value_pair_t *));
    if (!(table->pairs)) {
        free(table);
        table = NULL;
        return table;
    }

    {
        size_t i;
        for (i = 0; i < table->capacity; i++)
            table->pairs[i] = NULL;
    }

    return table;
}

static unsigned long _hash(char *string);
static key_value_pair_t * _pair_new(char *key, char *value);
static BOOL _expand(hash_table_t *self);

#define _PRMIE_SIZE  14
static size_t primes[_PRMIE_SIZE] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203,
    6421, 12853, 25717, 51437, 102877};

BOOL hash_table_add(hash_table_t *self, char *key, char *value)
{
    assert(self);

    if (!_expand(self))
        return FALSE;

    unsigned long code = _hash(key);
    size_t index = code % self->capacity;

    key_value_pair_t *p = NULL;
    key_value_pair_t *q = self->pairs[index];
    if (!q) {
        self->pairs[index] = _pair_new(key, value);
        if (!(self->pairs[index]))
            return FALSE;
    }
    else {
        while (q) {
            p = q;
            q = q->next;
        }

        p->next = _pair_new(key, value);
        if (!(p->next))
            return FALSE;
    }

    self->size += 1;

    return TRUE;
}

static unsigned long _hash(char *string)
{
    unsigned long code = 5381;

    size_t i;
    for (i = 0; i < strlen(string); i++)
        code = (code << 5) + code + string[i];

    return code;
}


static key_value_pair_t * _pair_new(char *key, char *value)
{
    key_value_pair_t *pair = \
        (key_value_pair_t *) malloc(sizeof(key_value_pair_t));
    if (!pair)
        return pair;

    pair->key = key;
    pair->value = value;
    pair->next = NULL;

    return pair;
}

static BOOL _rehash(hash_table_t *self, size_t capacity);

static BOOL _expand(hash_table_t *self)
{
    size_t threshold = self->capacity * 3 / 4;

    if (self->size <= threshold)
        return TRUE;

    if (self->capacity >= primes[_PRMIE_SIZE-1])
        return TRUE;

    size_t index = 0;
    {
        size_t i;
        for (i = 0; i < _PRMIE_SIZE; i++) {
            if (primes[i] > self->capacity) {
                index = i;
                break;
            }
        }
    }

    size_t capacity = primes[index];

    return _rehash(self, capacity);
}

static BOOL _rehash(hash_table_t *self, size_t capacity)
{
    key_value_pair_t **old_pairs = self->pairs;
    key_value_pair_t **new_pairs = \
        (key_value_pair_t **) \
        malloc(capacity * sizeof(key_value_pair_t *));
    if (!new_pairs)
        goto ERROR;

    {
        size_t i;
        for (i = 0; i < capacity; i++)
            new_pairs[i] = NULL;
    }

    {
        size_t i;
        for (i = 0; i < self->capacity; i++) {
            key_value_pair_t *p = old_pairs[i];

            while (p) {
                unsigned long code = _hash(p->key);
                size_t index = code % capacity;

                key_value_pair_t *x = NULL;
                key_value_pair_t *y = new_pairs[index];

                if (!y) {
                    new_pairs[index] = _pair_new(p->key, p->value);
                    if (!(new_pairs[index]))
                        goto ERROR;
                }
                else {
                    while (y) {
                        x = y;
                        y = y->next;
                    }

                    x->next = _pair_new(p->key, p->value);
                    if (!(x->next))
                        goto ERROR;
                }

                p = p->next;
            }
        }
    }

    size_t old_capacity = self->capacity;
    self->capacity = capacity;
    self->pairs = new_pairs;

    {
        size_t i;
        for (i = 0; i < old_capacity; i++) {
            key_value_pair_t *p = NULL;
            key_value_pair_t *q = old_pairs[i];

            while (q) {
                p = q;
                q = q->next;
                free(p);
            }
        }
    }

    free(old_pairs);

    return TRUE;

ERROR:
    if (new_pairs) {
        {
            size_t i;
            for (i = 0; i < capacity; i++) {
                key_value_pair_t *p = NULL;
                key_value_pair_t *q = new_pairs[i];

                while (q) {
                    p = q;
                    q = q->next;
                    free(p);
                }
            }
        }

        free(new_pairs);
    }

    return FALSE;
}

char * hash_table_get(hash_table_t *self, char *key)
{
    assert(self);

    unsigned long code = _hash(key);
    size_t index = code % self->capacity;

    key_value_pair_t *p;
    key_value_pair_t *q = self->pairs[index];

    if (!q) {
        return NULL;
    }

    while (q) {
        p = q;
        q = q->next;

        if (0 == strcmp(p->key, key)) {
            return p->value;
        }
    }

    return NULL;
}

static BOOL _shrink(hash_table_t *self);

BOOL hash_table_remove(hash_table_t *self, char *key)
{
    assert(self);

    if (!_shrink(self))
        return FALSE;

    unsigned long code = _hash(key);
    size_t index = code % self->capacity;

    key_value_pair_t *p = NULL;
    key_value_pair_t *q = self->pairs[index];

    if (!q)
        return FALSE;

    while (q) {
        if (0 == strcmp(key, q->key)) {
            if (!p) {
                if (!(q->next)) {
                    free(self->pairs[index]);
                    self->pairs[index] = NULL;
                }
                else {
                    self->pairs[index]->next = q->next;
                    free(q);
                }

            }
            else {
                p->next = q->next;
                free(q);
            }

            self->size -= 1;

            return TRUE;
        }

        p = q;
        q = q->next;
    }

    return FALSE;
}

static BOOL _shrink(hash_table_t *self)
{
    size_t threshold = self->capacity / 4;

    if (self->size >= threshold)
        return TRUE;

    if (self->capacity <= primes[0])
        return TRUE;

    size_t index = 0;
    {
        size_t i;
        for (i = 1; i < _PRMIE_SIZE; i++) {
            if (self->capacity >= primes[i]) {
                index = i - 1;
                break;
            }
        }
    }

    size_t capacity = primes[index];

    return _rehash(self, capacity);
}

void hash_table_delete(void *self)
{
    if (!self)
        return;

    key_value_pair_t **pairs = ((hash_table_t *) self)->pairs;
    size_t capacity = ((hash_table_t *) self)->capacity;

    {
        size_t i;
        for (i = 0; i < capacity; i++) {
            key_value_pair_t *p = NULL;
            key_value_pair_t *q = pairs[i];

            while (q) {
                p = q;
                q = q->next;
                free((void *) p);
            }
        }
    }

    free(((hash_table_t *) self)->pairs);
    free(self);
}
