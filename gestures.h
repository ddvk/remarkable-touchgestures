#pragma once
#include "eventreader.h"
#define TWOTAP_DISTANCE 700


void recognize_gestures(struct TouchEvent *f);
struct Gesture;
void interpret_gesture(struct Gesture *g);
