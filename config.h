#pragma once
//todo: get from touch device
#define SCREEN_WIDTH 1404
#define SCREEN_HEIGHT 1872

//todo: param or config
#ifdef REMARKABLE1
#define TOUCHSCREEN "/dev/input/event1"
#define TOUCH_WIDTH 767
#define TOUCH_HEIGHT 1023

#define BUTTONS "/dev/input/event2"
#define WACOM "/dev/input/event0"

#else

#define TOUCHSCREEN "/dev/input/event2"
#define TOUCH_WIDTH 1403
#define TOUCH_HEIGHT 1871

#define BUTTONS "/dev/input/event1"
#define WACOM "/dev/input/event1"

#endif

#define MAX_SLOTS 7  //max touch points to track
#define TWOTAP_DISTANCE 900 //distance between the fingers to enable disable
#define JITTER 10 //finger displacement to be consideded a swipe
#define SWIPE_VELOCITY 6 //swipe speed threshold
#define LONG_SWIPE 600 //long swipe distance
