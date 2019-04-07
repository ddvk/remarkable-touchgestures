#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>

#define TOUCHSCREEN "/dev/input/event1"
#define BUTTONS "/dev/input/event2"
#define MAX_SLOTS 5 
#define SCREEN_WIDTH 1404 
#define SCREEN_HEIGHT 1872


enum Key {Left=0,Right=1, Home=2};

enum FingerStatus {Down=0,Up=1,Move=2};

struct Finger {
    int x;
    int y;
    enum FingerStatus status;
    int track_id;
    bool state; //0 unused, 1 used
};

//fd
int buttons;

void emit(enum Key eventType){
    struct input_event key_input_event;
    memset(&key_input_event, 0, sizeof(key_input_event));
    int f = buttons; //TODO: global vars


    int code = 0;
    switch (eventType) {
        case Left:
            code = 105;
            break;
        case Right:
            code = 106;
            break;
        case Home:
            code = 102;
            break;

    }

    // key press event for 'a'
    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    key_input_event.value = 1;
    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    // EV_SYN for key press event
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.value = 0;

    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    key_input_event.value = 0;
    write(f, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    // EV_SYN for key press event
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.value = 0;

    write(f, &key_input_event,sizeof(key_input_event));
}

bool enabled = false;
int keys_down = 0;
bool multi_touch = 0;


struct Point { int x,y;};
struct Point segments[2];

void process_finger(struct Finger *f, struct timeval *t, int slot){
    if (f->status == Down){
        printf("slot %d down x:%d, y:%d  \n", slot, f->x, f->y);

        if(keys_down < 0) keys_down = 0; //todo: fixit

        keys_down++;
        if (keys_down>1){
            multi_touch= true;
        }
    }
    else if (f->status == Up){
        printf("slot %d up x:%d, y:%d  \n", slot, f->x, f->y);
        int x = f->x;
        int y = f->y;

        keys_down--;
        if (slot < 2){
            segments[slot].x = x;
            segments[slot].y = y;
        }


        if(multi_touch && keys_down == 0){
            int dx = segments[0].x - segments[1].x;
            int dy = segments[0].y - segments[1].y;
            int distance = sqrt(dx*dx+dy*dy);
            if (distance > 500) {
                if (enabled){
                    printf("disabling\n");
                }
                else {
                    printf("enabling\n");
                }
                enabled = !enabled;
            }
            multi_touch=false;
            return;
        }
        if (!enabled){
            return;
        }
        printf("slot %d x:%d, y:%d  \n", slot, f->x, f->y);

        int nav_stripe = SCREEN_WIDTH /3;
        if (x < nav_stripe) { 
            printf("Back\n");
            emit(Left);
        }
        else if (x > nav_stripe*2) {
            printf("Next\n");
            emit(Right);
        }
    }
}

void init() {
    buttons = open(BUTTONS,O_WRONLY);
    if (!buttons){
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }
}

void process_touch(){
    struct input_event evt;

    int slot = 0;
    int x,y = 0;
    int scr_width = 1404;
    int scr_height = 1872;
    int width = 767;
    int height = 1023;
    int pos = 0;

    struct Finger fingers[MAX_SLOTS];
    memset(fingers,0,sizeof(fingers));

    int touchscreen = open(TOUCHSCREEN, O_RDONLY);
    if (!touchscreen){
        fprintf(stderr, "cannot open touchscreen");
        exit(1);
    }
    while(read(touchscreen,&evt, sizeof(evt))){
        if (evt.type == EV_ABS) {
            if (evt.code == ABS_MT_SLOT) {
                slot = evt.value;
                if (slot >= MAX_SLOTS){
                    slot = MAX_SLOTS-1;
                }
                if (fingers[slot].state){
                    fingers[slot].status = Move;
                } 
                else {
                    fingers[slot].state = 1;
                    fingers[slot].status = Down;
                }
            }
            else if (evt.code == ABS_MT_POSITION_X) {
                float pos = width - 1 - evt.value;
                x = pos  / width  * scr_width;
                fingers[slot].x = x;
            }
            else if (evt.code == ABS_MT_POSITION_Y) {

                float pos = height - 1 - evt.value;
                y = pos  / height * scr_height;
                fingers[slot].y = y;
            } 
            else if (evt.code == ABS_MT_TRACKING_ID && evt.value == -1) {
                /* printf("slot %d tracking %d\n",slot, evt.value); */
                if (fingers[slot].state) {
                    fingers[slot].status = Up;
                }
            } 
            else if (evt.code == ABS_MT_TRACKING_ID) {
                if (slot == 0) {
                    /* printf("TRACK\n"); */
                    fingers[slot].state = 1;
                    fingers[slot].status = Down;
                }
                if (fingers[slot].state) {
                    fingers[slot].track_id = evt.value;
                }
            }
            else {
                /* printf("Uknown %d %d\n", evt.code, evt.value); */
            }
        }
        else if (evt.type == EV_SYN && evt.code == SYN_REPORT) {
            /* printf("SYN slot %d\n", slot); */
            struct Finger *f;
            for (int i=0;i< MAX_SLOTS;i++){
                f = &fingers[i];
                if (f->state) {

                    process_finger(f, &evt.time, i);

                    if (f->status == Down) {
                        f->status = Move;
                    } 

                    if (f->status == Up){
                        f->state = 0;
                    }

                }
            }

        }
    }
}

int main(){

    init();

    process_touch(); //TODO:function pointer

    return 0;
}
