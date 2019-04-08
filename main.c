#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>

#define VERSION "0.0.1"
#define TOUCHSCREEN "/dev/input/event1"
#define BUTTONS "/dev/input/event2"

#define MAX_SLOTS 5  //max touch points to track
#define MULTITOUCH_DISTANCE 500 //distance between the fingers to enable disable

#define SCREEN_WIDTH 1404 
#define SCREEN_HEIGHT 1872

#define TOUCH_WIDTH 767
#define TOUCH_HEIGHT 1023
#define JITTER 20 //finger displacement to be consideded a swipe

#define DEBUG 0
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

enum Key {Left, Right, Home};

enum FingerStatus {Down=0,Up=1,Move=2};

struct TouchEvent{
    int x,y,slot;
    long time;
    enum FingerStatus status;
};

//todo: rename to touch point or something
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

    debug_print("emitting %d \n", eventType);

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

    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    key_input_event.value = 1;
    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    write(f, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event,sizeof(key_input_event));
}

bool touch_enabled = false;
int keys_down = 0;
int segment_count = 0;
bool multi_touch = 0;


struct Point { 
    int x_start,y_start;
    int x,y;
};
struct Point segments[2];

void process_finger(struct TouchEvent *f){
    int slot = f->slot;
    if (f->status == Down){
        debug_print("slot %d down x:%d, y:%d  \n", slot, f->x, f->y);

        if(keys_down < 0) keys_down = 0; //todo: fixit
        if (slot < 2){
            segments[slot].x_start = f->x;
            segments[slot].y_start = f->y;
        }

        keys_down++;
        segment_count++;
    }
    else if (f->status == Up){
        debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, f->x, f->y, segment_count);
        int x = f->x;
        int y = f->y;

        keys_down--;
        if (slot < 2){
            segments[slot].x = x;
            segments[slot].y = y;
        }


        //todo: extract gesture recognition

        if(keys_down == 0){
            //single tap
            if (segment_count == 1) {
                
                segment_count=0;

                debug_print("tap slot %d x:%d, y:%d  \n", slot, f->x, f->y);
                struct Point *p = segments;
                int dx = p->x_start - p->x;
                int dy = p->y_start - p->y;


                if (touch_enabled){
                    if (abs(dx) < JITTER &&
                        abs(dy) < JITTER){

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
                    else {
                        //swipe 
                        if (abs(dx) > abs(dy)) {
                            //horizontal
                            if (dx < 0) {
                                printf("swipe left\n","");
                                //todo: output gestures, extract executer
                                emit(Right);
                            }
                            else {
                                printf("swipe right\n","");
                                emit(Left);
                            }
                        }
                        else {
                            //vertical
                        }
                    }
                }
            }
            else if(segment_count == 2){ //2 tap
                segment_count=0;
                int dx = segments[0].x - segments[1].x;
                int dy = segments[0].y - segments[1].y;
                int distance = sqrt(dx*dx+dy*dy);
                if (distance > 500) {
                    if (touch_enabled){
                        printf("disabling\n");
                    }
                    else {
                        printf("enabling\n");
                    }
                    touch_enabled = !touch_enabled;
                }
            }
            segment_count=0;
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


void process_touch(void(*process)(struct TouchEvent *)){
    struct input_event evt;

    int slot = 0;
    int x,y = 0;
    int scr_width = SCREEN_WIDTH;
    int scr_height = SCREEN_HEIGHT;
    int width = TOUCH_WIDTH;
    int height = TOUCH_HEIGHT;
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

                    struct TouchEvent event;
                    event.time = evt.time.tv_sec;
                    event.slot = i; //enumerating slots
                    event.x = f->x;
                    event.y = f->y;
                    event.status = f->status;

                    process(&event);

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

int main() {
    printf("touchinjector %s\n",VERSION);

    init();

    process_touch(&process_finger);

    return 0;
}
