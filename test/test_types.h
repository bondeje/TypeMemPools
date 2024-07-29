#include <stdint.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

struct sa11 {int8_t data[1];};
struct sa21 {int8_t data[2];};
struct sa31 {int8_t data[3];};
struct sa41 {int8_t data[4];};

struct sa22 {int16_t data[1];};
struct sa32 {int16_t data[1]; char off;};
struct sa42 {int16_t data[2];};
struct sa62 {int16_t data[3];};
struct sa82 {int16_t data[4];};

struct sa44 {int32_t data[1];};
struct sa54 {int32_t data[1]; char off;};
struct sa84 {int32_t data[2];};
struct sa124 {int32_t data[3];};
struct sa164 {int32_t data[4];};

struct sa88 {int64_t data[1];};
struct sa98 {int64_t data[1]; char off;};
struct sa168 {int64_t data[2];};
struct sa248 {int64_t data[3];};
struct sa328 {int64_t data[4];};

#if __SIZEOF_LONG_DOUBLE__ > 8
struct sa1616 {long double data[1];};
struct sa1716 {long double data[1]; char off;};
struct sa3216 {long double data[2];};
struct sa4816 {long double data[3];};
struct sa6416 {long double data[4];};
#endif

