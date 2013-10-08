#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

#include <stdio.h>

#define info(format) fprintf(stderr, "%s:%d:%s -> " format "\n", __FILE__, __LINE__, __func__)
#define log(format, ...) fprintf(stderr, "%s:%d:%s -> " format "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)

#else

#define info(...)
#define log(...)

#endif //DEBUG

#endif //DEBUG_H
