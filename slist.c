#include "slist.h"
#include "assert.h"
#include "azlib/general.h"

typedef struct Node {
    struct Node *next;
    void *item;
} Node;

typedef struct SList {
    Node *first;
    Node *last;
    Node *cursor;
    size_t size;
    slist_cmp_fun *cmp_fun;
    slist_free_fun *free_fun;    
    slist_cpy_fun *cpy_fun;
} SList;


// int get_num_memcopies(SList* self, Node *node)
// {
//     assert(self);
//     assert(node);
//     Node *node2;
//     int num_instances = 0;
//     for (node2 = self->first; node2 != 0; node2 = node2->next)
//     {
//         if (node2->item == node->item)
//             num_instances += 1;
//     }
//     return num_instances;
// }

int slist_append(SList *self, void *item)
{
    assert(self);

	if (!item)
        return -1;

    Node *node;
    node = (Node*)azmalloc(sizeof(Node));

    if (self->free_fun)
    {
        if (!self->cpy_fun)
        {
            errno = EINVAL;
            return EINVAL;
        }
        node->item = (*self->cpy_fun)(item);
    }
    else
        node->item = item;

    if (self->last)
        self->last->next = node;
    else
        self->first = node;
    self->last = node;

    self->size++;
    // self->cursor = 0; why?
    return 0;
}

void slist_clear(SList *self)
{
	assert(self);
    // Node *node = self->first;
    // if (self->free_fun)
    // {
    //     for (node = self->first; node != 0; node = node->next)
    //     {
    //         assert(node->item);
    //         if (!node->item)
    //             continue;
    //         int num_memcopies = get_num_memcopies(self, node);
    //         assert(num_memcopies > 0);
    //         if (num_memcopies == 1)
    //         {
    //             (*self->free_fun)(node->item);
    //         }
    //         node->item = 0;
    //     }
    // }
    // node = self->first;
    // while (node)
    // {
    //     Node *next = node->next;
    //     free(node);
    //     node = next;
    // }

    Node *node = self->first;
    while (node)
    {
        Node *next = node->next;
        if (self->free_fun)
            (*self->free_fun)(node->item);
        free(node);
        node = next;
    }

    self->first = 0;
    self->last = 0;
    self->cursor = 0;
    self->size = 0;
}

void* slist_current(SList *self)
{
	assert(self);
    return self->cursor ? self->cursor->item : 0;
}

SList* slist_copy(SList *self)
{
    assert(self);

    if (self->free_fun && !self->cpy_fun)
    {
        errno = EINVAL;
        return 0;
    }

    SList *copy = slist_new();
    copy->free_fun = self->free_fun;
    copy->cmp_fun = self->cmp_fun;
    copy->cpy_fun = self->cpy_fun;
    Node *node;
    for (node = self->first; node; node = node->next)
    {
        assert(slist_append(copy, node->item) == 0);
    }
    copy->cursor = self->cursor;
    return copy;
}


void slist_destroy(SList **self_p)
{
	assert(self_p);
    if (*self_p) {
        SList *self = *self_p;
        slist_clear(self);
        free(self);
        *self_p = 0;
    }
}

bool slist_exists(SList *self, void* item)
{
    assert(self);
    assert(item);
    Node *node = self->first;

    while (node)
    {
        if (self->cmp_fun)
        {
            if ((*self->cmp_fun)(node->item, item) == 0)
                return true;
        }
        else
        {
            if (node->item == item)
                return true;
        }
        node = node->next;
    }
    return false;
}

void* slist_first(SList *self)
{
    assert(self);
    return self->first ? self->first->item : 0;
}

void* slist_last(SList *self)
{
    assert(self);
    return self->last ? self->last->item : 0;
}

SList* slist_new()
{
	SList *self = (SList*)azmalloc(sizeof(SList));
    return self;
}

void* slist_next(SList *self)
{
	assert(self);
    if (self->cursor)
        self->cursor = self->cursor->next;
    else
        self->cursor = self->first;
    return self->cursor ? self->cursor->item : 0;
}

void* slist_pop(SList *self)
{
    Node *node = self->first;
    void *item = 0;
    if (node) {
        if (self->cursor == node)
            self->cursor = 0;
        item = node->item;
        self->first = node->next;
        if (self->last == node)
           self->last = 0;
        free(node);
        self->size--; 
    }
    // self->cursor = 0;
    return item;
}

int slist_prepend(SList *self, void *item)
{
    assert(self);

	if (!item)
        return -1;

    Node *node;
    node = (Node*)azmalloc(sizeof(Node));

    if (self->free_fun)
    {
        if (!self->cpy_fun)
        {
            errno = EINVAL;
            return EINVAL;
        }
        node->item = (*self->cpy_fun)(item);
    }
    else
        node->item = item;
    node->next = self->first;
    self->first = node;
    if (self->last == 0)
        self->last = node;

    self->size++;
    // self->cursor = 0; why?
    return 0;
}


int slist_remove(SList *self, void* item)
{
	Node *node, *prev = 0;

    for (node = self->first; node != 0; node = node->next)
    {
        if (self->cmp_fun) {
            if ((*self->cmp_fun)(node->item, item) == 0)
                break;
        }
        else {
            if (node->item == item)
                break;
        }
        prev = node;
    }

    if (!node)
        return -1;

    if (prev)
        prev->next = node->next;
    else
        self->first = node->next;

    if (node->next == 0)
        self->last = prev;

    if (self->cursor == node)
        self->cursor = prev;

    // if (self->free_fun)
    // {
    //     int num_memcopies = get_num_memcopies(self, node);
    //     assert(num_memcopies > 0);
    //     if (num_memcopies == 1)
    //         (*self->free_fun)(node->item);
    // }

    free(node);
    self->size--;
    return 0;
}

void slist_set_cmpfun(SList* self, slist_cmp_fun cmp_fun)
{
    assert(self);
    self->cmp_fun = cmp_fun;
}

void slist_set_free_cpy(SList *self, slist_free_fun free_fun, slist_cpy_fun cpy_fun)
{
    assert(self);
    assert(self->size == 0);
    assert((free_fun && cpy_fun) || (!free_fun && !cpy_fun));
    self->free_fun = free_fun;
    self->cpy_fun = cpy_fun;
}

size_t slist_size(SList *self)
{
	assert(self);
    return self->size;
}

//  Uses a comb sort, which is simple and reasonably fast.
//  See http://en.wikipedia.org/wiki/Comb_sort
void slist_sort(SList *self)
{
    assert(self);
    assert(self->cmp_fun);

    size_t gap = self->size;
    bool swapped = false;
    while (gap > 1 || swapped)
    {
        if (gap > 1)
            gap = (size_t)((double)gap / 1.3);
        Node *base = self->first;
        Node *test = self->first;
        size_t jump = gap;
        while (jump--)
            test = test->next;

        swapped = false;
        while (base && test)
        {
            if ((*self->cmp_fun)(base->item, test->item) > 0)
            {
                void *item = base->item;
                base->item = test->item;
                test->item = item;
                swapped = true;
            }
            base = base->next;
            test = test->next;
        }
    }
}
