#pragma once
#include "point.h"

enum GestureType {
    Tap, 
    TapLeft,
    TapRight,
    TapHold,
    TwoTap,
    TwoTapWide,
    SwipeLeft,
    SwipeRight,
    SwipeUp,
    SwipeUpLong,
    SwipeDown,
    SwipeDownLong
};

struct Tap {
    struct Point p;
    int hold_time;
};
struct TwoTap {
    struct Point start;
    struct Point end;
};
struct Swipe {
    struct Point start;
    struct Point end;
    int velocity;
};
union GestureData {
    struct TwoTap twotap;
    struct Swipe swipe;
};

struct Gesture {
    enum GestureType type;
    union GestureData data;
};
