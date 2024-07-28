/* This is a set of utilities that can be used across test environments */

extern _Bool verbose;

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)
#define CAT3_(a, b, c) a##b##c
#define CAT3(a, b, c) CAT3_(a, b, c)

#define CHECK(result, failmsg, ...) check(result, "%s-%s-%zu: " failmsg, __FILE__, __func__, __LINE__, __VA_ARGS__)
int check(_Bool result, char const * failmsg, ...);