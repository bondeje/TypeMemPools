#include <stddef.h>

typedef struct MemPool MemPool;
typedef struct MemPoolManager MemPoolManager;

MemPoolManager * MemPoolManager_new(size_t count, size_t size, size_t alignment);
void MemPoolManager_del(MemPoolManager * self);
void * MemPoolManager_next(MemPoolManager * self);
void * MemPoolManager_nextsize(MemPoolManager * self, size_t size);
void * MemPoolManager_nextaligned(MemPoolManager * self, size_t size, size_t alignment);