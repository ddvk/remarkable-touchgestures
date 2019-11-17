#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ui.h"
#include "gestures.h"
#include "keyinjector.h"
#include "gesture_definition.h"
#include "eventreader.h"
#include "config.h"

static int keys_down = 0;
static int segment_count = 0;

struct Segment { 
    struct Point start;
    struct Point end;
};

static struct Segment segments[MAX_SLOTS+1];

int get_distance(struct Point *start, struct Point *end, int *angle, unsigned int *velocity){
    int dx = end->x - start->x;
    int dy = end->y - start->y;
    unsigned long dt = end->time - start->time;

    //todo: fix the math
    if (abs(dx) > abs(dy)) {
        if (dx > 0)
            *angle=0;
        else
            *angle=180;
    }
    else {
        if (dy > 0)
            *angle=270;
        else
            *angle=90;
    }
    int distance =(int) sqrt(dx*dx+dy*dy);
    *velocity = (10*distance) / (dt == 0 ? 1 : dt);
    return distance; 
}

void recognize_gestures(struct TouchEvent *f) {
    int slot = f->slot;
    if (f->status == Down){
        debug_print("slot %d down x:%d, y:%d  \n", slot, f->x, f->y);

        if(keys_down < 0) keys_down = 0; //todo: fixit
        segments[slot].start.x = f->x;
        segments[slot].start.y = f->y;
        segments[slot].start.time = f->time;

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
        segments[slot].end.time = f->time;

        //todo: extract gesture recognition

        if(keys_down == 0){
            struct Gesture gesture;
            struct Segment *p = segments;
            int angle,distance,distance1=0;
            unsigned int velocity=0;
            switch(segment_count){

                //single tap
                case 1: 
                    printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", f->x, f->y, f->raw_position.x, f->raw_position.y);
                    distance = get_distance(&p->start,&p->end,&angle,&velocity);

                    if (distance < JITTER) {
                        int nav_stripe = SCREEN_WIDTH /3;
                        if (y > 100 && x > 100){//disable upper stripe and left menus
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
                        printf("velocity %d\n", velocity);
                        if (velocity < SWIPE_VELOCITY) //ignore slow swipes
                            break;
                        //swipe 
                        switch(angle) {
                            case 0:
                                printf("swipe right\n");
                                gesture.type = SwipeRight; 
                                interpret_gesture(&gesture);
                                break;
                            case 180:
                                printf("swipe left\n");
                                gesture.type = SwipeLeft; 
                                interpret_gesture(&gesture);
                                break;
                            case 90:
                                if (distance > LONG_SWIPE) 
                                    gesture.type = SwipeUpLong;
                                else
                                    gesture.type = SwipeUp;

                                interpret_gesture(&gesture);
                                break;
                            case 270:
                                if (distance > LONG_SWIPE)
                                    gesture.type = SwipeDownLong; 
                                else
                                    gesture.type = SwipeDown; 

                                interpret_gesture(&gesture);
                                break;
                            default:
                                fprintf(stderr,"unknown angle: %d\n", angle);
                                break;
                        }
                    }
                    break;
                case 2:
                    distance1 = get_distance(&segments[0].start, &segments[1].start, &angle, &velocity);
                    distance = get_distance(&segments[0].end, &segments[1].end, &angle, &velocity);

                    int distance_delta = distance - distance1; //zoom in, out
                    if (distance > TWOTAP_DISTANCE && abs(distance_delta) < JITTER) {
                        printf("change mode\n");
                        gesture.type = TwoTapWide;
                        interpret_gesture(&gesture);
                        break;
                    }
                    //pinch zoom out
                    if (distance_delta > 0) {
                        break;
                    }
                    break;
                default:
                    printf("%d finger\n",segment_count);
                    break;
            }
            segment_count=0;
        }
    }
}
