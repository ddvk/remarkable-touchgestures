#pragma once
struct Gesture;
struct TouchEvent;
void recognize_gestures(struct TouchEvent *f);
void interpret_gesture(struct Gesture *g);
