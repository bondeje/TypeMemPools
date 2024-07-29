#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "test_types.h"
#include "test_mempoolmgr.h"
#include "test_utils.h"

#define PRINT_SIZE_ALIGN(type) printf(STRINGIFY(type) " - size: %zu, align: %zu\n", sizeof(type), _Alignof(type))

int main(int narg, char ** args) {
    for (int i = 1; i < narg; i++) {
        if (!strcmp(args[i], "--showtypes")) {
            PRINT_SIZE_ALIGN(struct sa11);
            PRINT_SIZE_ALIGN(struct sa21);
            PRINT_SIZE_ALIGN(struct sa31);
            PRINT_SIZE_ALIGN(struct sa41);

            PRINT_SIZE_ALIGN(struct sa22);
            PRINT_SIZE_ALIGN(struct sa32);
            PRINT_SIZE_ALIGN(struct sa42);
            PRINT_SIZE_ALIGN(struct sa62);
            PRINT_SIZE_ALIGN(struct sa82);

            PRINT_SIZE_ALIGN(struct sa44);
            PRINT_SIZE_ALIGN(struct sa54);
            PRINT_SIZE_ALIGN(struct sa84);
            PRINT_SIZE_ALIGN(struct sa124);
            PRINT_SIZE_ALIGN(struct sa164);

            PRINT_SIZE_ALIGN(struct sa88);
            PRINT_SIZE_ALIGN(struct sa98);
            PRINT_SIZE_ALIGN(struct sa168);
            PRINT_SIZE_ALIGN(struct sa248);
            PRINT_SIZE_ALIGN(struct sa328);
    #if __SIZEOF_LONG_DOUBLE__ > 8
            PRINT_SIZE_ALIGN(struct sa1616);
            PRINT_SIZE_ALIGN(struct sa1716);
            PRINT_SIZE_ALIGN(struct sa3216);
            PRINT_SIZE_ALIGN(struct sa4816);
            PRINT_SIZE_ALIGN(struct sa6416);
    #endif
        } else if (!strcmp(args[i], "--verbose")) {
            verbose = 1;
        }
    }
    int nerrors = test_mempoolmgr_new_del();
    nerrors += test_mempoolmgr_next();
    nerrors += test_mempoolmgr_malloc();
    nerrors += test_mempoolmgr_aligned_alloc();

    printf("Mempool tests %s! (%d errors)!\n", nerrors ? "failed" : "passed", nerrors);
    return nerrors;
}

