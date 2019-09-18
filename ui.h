#pragma once
#include <stdbool.h>


#define DEBUG 0
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#ifdef __cplusplus
extern "C" {
#endif
void show(const char *str);
void ui_init();

#ifdef __cplusplus
}
#endif
