#include <stddef.h>
#include <stdio.h>

#include "test_utils.h"
#include "test_types.h"
#include "test_mempoolmgr.h"

#define MAX_TEST_COUNTS 16384
#define MAX_TEST_SIZE 16384
#define MAX_TEST_ALIGN __BIGGEST_ALIGNMENT__

_Bool is_aligned(void * ptr, unsigned char align) {
    return ((uintptr_t)ptr) % align == 0;
}

int test_mempoolmgr_new_del(void) {
    int nerrors = 0;

    for (size_t cts = 1; cts <= MAX_TEST_COUNTS; cts *= 2) {
        for (size_t size = 1; size <= MAX_TEST_SIZE; size *= 2) {
            for (size_t align = 1; align <= MAX_TEST_ALIGN; align *= 2) {
                MemPoolManager * mgr = MemPoolManager_new(cts, size, align);
                nerrors += CHECK(mgr != NULL, "failed to allocate MemPoolManager for (count, size, alignment)=(%zu, %zu, %zu)", cts, size, align);
                MemPoolManager_del(mgr);
            }
        }
    }

    if (verbose) {
        printf("test_mempoolmgr_new_del...%s!\n", nerrors ? "failed" : "passed");
    }
    
    return nerrors;
}

int test_mempoolmgr_next(void) {
    int nerrors = 0;

#define MEMPOOLMGR_NEXT_TEST(type)\
    for (size_t cts = 1; cts <= MAX_TEST_COUNTS; cts *= 2) {\
        MemPoolManager * mgr = MemPoolManager(type, cts);\
        type * last = MemPoolManager_next(mgr);\
        nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)last, _Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
        for (size_t i = 1; i < cts; i++) {\
            type * next = MemPoolManager_next(mgr);\
            nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
            nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
            nerrors += CHECK((size_t)((char*)next - (char*)last) == sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
            last = next;\
        }\
        /* this one should be in a new MemPool */\
        type * next = MemPoolManager_next(mgr);\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != sizeof(type), "subsequent " STRINGIFY(type) " past count is not in a new pool: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        MemPoolManager_del(mgr);\
    }

    MEMPOOLMGR_NEXT_TEST(char)
    MEMPOOLMGR_NEXT_TEST(short)
    MEMPOOLMGR_NEXT_TEST(double)
    MEMPOOLMGR_NEXT_TEST(long double)
    MEMPOOLMGR_NEXT_TEST(struct sa31)
    MEMPOOLMGR_NEXT_TEST(struct sa32)
    MEMPOOLMGR_NEXT_TEST(struct sa54)
    MEMPOOLMGR_NEXT_TEST(struct sa98)
#if __SIZEOF_LONG_DOUBLE__ > 8
    MEMPOOLMGR_NEXT_TEST(struct sa1716)
#endif

    if (verbose) {
        printf("test_mempoolmgr_next...%s!\n", nerrors ? "failed" : "passed");
    }

    return nerrors;
}

int test_mempoolmgr_malloc(void) {
    int nerrors = 0;

#define MEMPOOLMGR_MALLOC_TEST(type)\
    for (size_t array_size = 1; array_size <= MAX_TEST_COUNTS / 2; array_size *= 2) {\
        /* allocating an array at a time, each pool has space for 2 arrays sequentially*/\
        MemPoolManager * mgr = MemPoolManager(type, array_size * 2);\
        type * last = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)last, _Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
        type * next = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) == array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        last = next;\
        next = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        last = next;\
        next = MemPoolManager_malloc(mgr, (1+array_size) * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        MemPoolManager_del(mgr);\
    }

    MEMPOOLMGR_MALLOC_TEST(char)
    MEMPOOLMGR_MALLOC_TEST(short)
    MEMPOOLMGR_MALLOC_TEST(double)
    MEMPOOLMGR_MALLOC_TEST(struct sa31)
    MEMPOOLMGR_MALLOC_TEST(struct sa32)
    MEMPOOLMGR_MALLOC_TEST(struct sa54)
    MEMPOOLMGR_MALLOC_TEST(struct sa98)
#if __SIZEOF_LONG_DOUBLE__ > 8
    MEMPOOLMGR_MALLOC_TEST(long double)
    MEMPOOLMGR_MALLOC_TEST(struct sa1716)
#endif

    if (verbose) {
        printf("test_mempoolmgr_malloc...%s!\n", nerrors ? "failed" : "passed");
    }

    return nerrors;
}

// TODO: this test needs work
int test_mempoolmgr_aligned_alloc(void) {
    int nerrors = 0;

// test 2, 4, 8, 16 (if available)
#define MEMPOOLMGR_ALIGNED_ALLOC_TEST(type) {\
    MemPoolManager * mgr = MemPoolManager_new(2, sizeof(type), _Alignof(type));\
    /* mis-aligning object */\
    struct sa11 * last = MemPoolManager_malloc(mgr, sizeof(struct sa11));\
    nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
    nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)last, _Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
    /* aligned object */\
    void * next = MemPoolManager_aligned_alloc(mgr, sizeof(type), _Alignof(type));\
    nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n", NULL);\
    nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %d = %zu\n", (void*)next, _Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
    /* setup is intentially so that 'next' is not aligned to next higher alignment */\
    nerrors += CHECK((size_t)((char*)next - (char*)last) == _Alignof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
    MemPoolManager_del(mgr); }

    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 2, 2))
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 4, 4))
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 8, 8))
#if __BIGGEST_ALIGNMENT__ > 8
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 16, 16))
#endif

    if (verbose) {
        printf("test_mempoolmgr_aligned_alloc...%s!\n", nerrors ? "failed" : "passed");
    }

    return nerrors;
}