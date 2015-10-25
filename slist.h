#ifndef __SLIST_H_INCLUDED__
#define __SLIST_H_INCLUDED__

#include <stdlib.h>
#include <stdbool.h>

typedef int (slist_cmp_fun)(void *item1, void *item2);
typedef void (slist_free_fun)(void *data);
typedef void* (slist_cpy_fun)(void *data);

typedef struct SList SList;

SList* slist_new();

void slist_destroy(SList **self_p);

void slist_clear(SList *self);

void* slist_first(SList *self);

void* slist_last(SList *self);

void* slist_next(SList *self);

void* slist_current(SList *self);

int slist_append(SList *self, void *item);

int slist_prepend(SList *self, void *item);

void* slist_pop(SList *self);

bool slist_exists(SList *self, void* item);

int slist_remove(SList *self, void* item);

SList* slist_copy(SList *self);

size_t slist_size(SList *self);

void slist_sort(SList *self);

void slist_set_cmpfun(SList *self, slist_cmp_fun cmp_fun);

void slist_set_free_cpy(SList *self, slist_free_fun free_fun, slist_cpy_fun cpy_fun);

#endif /* end of include guard: __SLIST_H_INCLUDED__ */
