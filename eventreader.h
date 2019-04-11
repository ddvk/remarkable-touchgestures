#pragma once
#include <stdbool.h>
enum FingerStatus {Untracked=0,Down,Up,Move};

struct Point {
    int x,y;
};
struct TouchEvent{
    int x,y,slot;
	struct Point position;
	struct Point raw_position;

    long time;
    enum FingerStatus status;
};

struct Finger {
    int x;
    int y;
	int raw_x;
	int raw_y;
    enum FingerStatus status;
    int track_id;
};

void process_touch(void(*process)(struct TouchEvent *));
