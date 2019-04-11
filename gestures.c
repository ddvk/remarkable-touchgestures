#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ui.h"
#include "gestures.h"
#include "keyinjector.h"

enum Key {Left=105, Right=106, Home=102,Power=116};

static int keys_down = 0;
static int segment_count = 0;

struct Segment { 
    struct Point start;
    struct Point end;
};
struct Segment segments[MAX_SLOTS+1];

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
                                    myemit(Left);
                                }
                            }
                            else if (x > nav_stripe*2) {
                                myemit(Right);
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
                                myemit(Right);
                            }
                            else {
                                printf("swipe right\n");
                                myemit(Left);
                            }
                        }
                        else {
                            //vertical
                            if (dy > 0 && dy > 600) {
                                //down
                                myemit(Power);
                            }
                            else if (dy < 0 && dy < -600) {
                                //todo: move
                                time_t rawtime;
                                time(&rawtime);
                                struct tm* timeinfo = localtime(&rawtime);
                                char buffer[100];
                                strftime(buffer,sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
                                show(buffer);
                            }
                        }
                    }
                    break;
                case 2:
                    dx = segments[0].end.x - segments[1].end.x;
                    dy = segments[0].end.y - segments[1].end.y;
                    distance = sqrt(dx*dx+dy*dy);
                    if (distance > 500) {
                        if (touch_enabled){
                            show("touch navigation disabled");
                            printf("disabling\n");
                            touch_enabled = false;
                        }
                        else {
                            show("touch navigation enabled");
                            printf("enabling\n");
                            touch_enabled = true;
                        }
                    }
                    break;
                case 3:
                    printf("3 finger tap\n");
                    break;
                case 4:
                    printf("4\n");
                    break;
            }
            segment_count=0;
        }
    }
}
