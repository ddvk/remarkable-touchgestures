#pragma once
#include "point.h"
enum FingerStatus {Untracked=0,Down,Up,Move};


struct TouchEvent{
    int x,y,slot;
	struct Point position;
	struct Point raw_position;

    unsigned long time;
    enum FingerStatus status;
};

void process_touch(void(*process)(struct TouchEvent *));
