#pragma once
#include <stdbool.h>

#define SCREEN_WIDTH 1404 
#define SCREEN_HEIGHT 1872
#define TOUCH_WIDTH 767
#define TOUCH_HEIGHT 1023

#define TOUCHSCREEN "/dev/input/event1"
#define BUTTONS "/dev/input/event2"
#define WACOM "/dev/input/event0"

#define MAX_SLOTS 7  //max touch points to track
#define MULTITOUCH_DISTANCE 500 //distance between the fingers to enable disable

#define JITTER 20 //finger displacement to be consideded a swipe

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
