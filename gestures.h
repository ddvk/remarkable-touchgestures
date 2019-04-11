#pragma once
#include "eventreader.h"

enum GestureType {
    Tap, 
    TwoFingerTap,TwoFingerWide, SwipeLeft, SwipeRight, SwipeUp,SwipeDown};

union GestureData {
    struct Point start;
    struct Point end;
};

struct Gesture {
    enum GestureType type;
    union GestureData data;
};

void recognize_gestures(struct TouchEvent *f);

