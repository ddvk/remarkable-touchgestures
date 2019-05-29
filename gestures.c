#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ui.h"
#include "gestures.h"
#include "keyinjector.h"
#include "gesture_definition.h"

static int keys_down = 0;
static int segment_count = 0;

static struct Segment { 
    struct Point start;
    struct Point end;
};

static struct Segment segments[MAX_SLOTS+1];

void recognize_gestures(struct TouchEvent *f) {
    int slot = f->slot;
    if (f->status == Down){
        debug_print("slot %d down x:%d, y:%d  \n", slot, f->x, f->y);

        if(keys_down < 0) keys_down = 0; //todo: fixit
        segments[slot].start.x = f->x;
        segments[slot].start.y = f->y;

        keys_down++;
        segment_count++;
    }
    else if (f->status == Up){
        debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, f->x, f->y, segment_count);
        int x = f->x;
        int y = f->y;

        keys_down--;
        segments[slot].end.x = x;
        segments[slot].end.y = y;

        //todo: extract gesture recognition

        if(keys_down == 0){
            struct Gesture gesture;
            struct Segment *p = segments;
            int dx,dy,distance=0;
            switch(segment_count){
                //single tap
                case 1: 
                    printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", f->x, f->y, f->raw_position.x, f->raw_position.y);
                    dx = p->end.x - p->start.x;
                    dy = p->end.y - p->start.y;

                    if (abs(dx) < JITTER &&
                            abs(dy) < JITTER){

                        int nav_stripe = SCREEN_WIDTH /3;
                        if (y > 100){//disable upper stripe 
                            if (x < nav_stripe) { 
                                if (y > SCREEN_HEIGHT - 150 && x < 150) {
                                    printf("TOC\n");
                                }
                                else {

                                    gesture.type = TapLeft; 
                                    interpret_gesture(&gesture);
                                }
                            }
                            else if (x > nav_stripe*2) {
                                    gesture.type = TapRight; 
                                    interpret_gesture(&gesture);
                            }
                        }
                    }
                    else {
                        //swipe 
                        if (abs(dx) > abs(dy)) {
                            //horizontal
                            if (dx < 0) {
                                printf("swipe left\n");
                                //todo: output gestures, extract executer
                                    gesture.type = SwipeLeft; 
                                    interpret_gesture(&gesture);
                            }
                            else {
                                printf("swipe right\n");
                                gesture.type = SwipeRight; 
                                interpret_gesture(&gesture);
                            }
                        }
                        else {
                            //vertical
                            if (dy > 0 && dy > 600) {
                                gesture.type = SwipeDownLong; 
                                interpret_gesture(&gesture);
                            }
                            else if (dy < 0 && dy < -600) {
                                gesture.type = SwipeUpLong;
                                interpret_gesture(&gesture);
                            }
                        }
                    }
                    break;
                case 2:
                    dx = segments[0].end.x - segments[1].end.x;
                    dy = segments[0].end.y - segments[1].end.y;
                    distance = sqrt(dx*dx+dy*dy);
                    if (distance > TWOTAP_DISTANCE) {
                        gesture.type = TwoTapWide;
                        interpret_gesture(&gesture);
                    }
                    break;
                default:
                    printf("%d finger tap\n",segment_count);
                    break;
            }
            segment_count=0;
        }
    }
}
