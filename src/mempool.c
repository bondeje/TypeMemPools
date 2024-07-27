#include <stdlib.h>

#include "mempool.h"

struct MemPoolManager {
    MemPool * tail;             // final mempool. represents stack of pools
    size_t size;                // size of a standard element
    size_t memory_size;         // must be at least min_el_size
    size_t alignment_offset;
};

struct MemPool {
    char * memory;
    size_t memory_size; // number of bytes in memory
    MemPool * prev; // point to next memory pool
    size_t memory_consumed;
};

void MemPool_init(MemPool * self, char * memory, size_t memory_size, MemPool * prev) {
    self->memory = memory;
    self->memory_size = memory_size;
    self->memory_consumed = 0;
    self->prev = prev;
}

MemPool * MemPool_new(size_t memory_size, size_t alignment_offset, MemPool * prev) {
    MemPool * pool = malloc(alignment_offset + memory_size);
    if (pool) {
        MemPool_init(pool, ((char *)pool) + alignment_offset, memory_size, prev);
    }
    return pool;
}

void MemPool_del(MemPool * self) {
    free(self);
}

void * MemPool_nextsize(MemPool * self, size_t size) {
    if (self->memory_size - self->memory_consumed < size) {
        return NULL;
    }
    void * out = (void *) (self->memory + self->memory_consumed);
    self->memory_consumed += size;
    return out;
}

void * MemPool_nextaligned(MemPool * self, size_t size, size_t alignment) {
    size_t offset = (alignment - 1 - ((((uintptr_t)(void *)self->memory) - 1) % alignment)) % alignment;
    if (SIZE_MAX - self->memory_consumed < offset || self->memory_size - self->memory_consumed - offset < size) {
        return NULL;
    }
    self->memory_consumed += offset;
    void * out = (void *) (self->memory + self->memory_consumed);
    self->memory_consumed += size;
    return out;
}

void MemPoolManager_init(MemPoolManager * self, MemPool * tail, size_t count, size_t size, size_t alignment_offset) {
    self->tail = tail;
    self->size = size;
    self->memory_size = size * count;
    self->alignment_offset = alignment_offset;
}

MemPoolManager * MemPoolManager_new(size_t count, size_t size, size_t alignment) {
    if (!count || !size) {
        return NULL;
    }
    if (!alignment) {
        alignment = __BIGGEST_ALIGNMENT__; // this only exists for clang/gcc for sure
    }
    
    size_t alignment_offset = alignment * ((sizeof(MemPool) - 1) / alignment + 1);
    
    MemPoolManager * new  = malloc(sizeof(*new));
    if (new) {
        MemPool * tail = MemPool_new(size * count, alignment_offset, NULL);
        if (!tail) {
            free(new);
            return NULL;
        }
        MemPoolManager_init(new, tail, count, size, alignment_offset);
    }
    return new;
}

void MemPoolManager_del(MemPoolManager * self) {
    MemPool * tail = self->tail;
    while (tail) {
        MemPool * next = tail->prev;
        MemPool_del(tail);
        tail = next;
    }
    self->tail = NULL;
    free(self);
}

void * MemPoolManager_next(MemPoolManager * self) {
    return MemPoolManager_nextsize(self, self->size);    
}

void * MemPoolManager_nextsize(MemPoolManager * self, size_t size) {
    void * out = MemPool_nextsize(self->tail, size);
    if (!out) {
        // try to add a new MemPool
        self->tail = MemPool_new(self->memory_size, self->alignment_offset, self->tail);
        out = MemPool_nextsize(self->tail, size);
    }
    return out;
}

void * MemPoolManager_nextaligned(MemPoolManager * self, size_t size, size_t alignment) {
    void * out = MemPool_nextaligned(self->tail, size, alignment);
    if (!out) {
        // try to add a new MemPool
        self->tail = MemPool_new(self->memory_size, self->alignment_offset, self->tail);
        out = MemPool_nextaligned(self->tail, size, alignment);
    }
    return out;
}