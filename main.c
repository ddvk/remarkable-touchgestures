#include<stdio.h>
#include<stdlib.h>
#include<linux/input.h>
#include<memory.h>
#include <unistd.h>
#include <fcntl.h>

#define TOUCHSCREEN "/dev/input/event1"
#define BUTTONS "/dev/input/event2"
#define MAX_SLOTS 10


enum Key {Left=0,Right=1, Home=2};

enum FingerStatus {Down=0,Up=1,Move=2};

struct Finger {
	int x;
	int y;
	enum FingerStatus status;
	int track_id;
	int state; //0 unused, 1 used
};
void report(struct Finger *f, struct timeval *t, int slot){
    if (f->status == Move){
    }
    else
        printf("slot %d x:%d, y:%d  status %d\n", slot, f->x, f->y, f->status);
}

void emit(int f, enum Key eventType){
 	struct input_event key_input_event;
    memset(&key_input_event, 0, sizeof(key_input_event));


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

int main(){
    int buttons = open(BUTTONS,O_WRONLY);
    if (!buttons){
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }

	/* emit(buttons, Right); */

    struct input_event evt;

    int slot=0;
    int x,y=0;
    int scr_width = 1404;
    int scr_height = 1872;
    int width  = 767;
    int height  = 1023;
    int pos=0;

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
                printf("SLOT\n");
                slot = evt.value;
                if (slot >= MAX_SLOTS){
                    slot = MAX_SLOTS-1;
                }
                printf("slot %d\n",slot);
                if (fingers[slot].state){
                    fingers[slot].status = Move;
                } 
                else {
                    fingers[slot].state = 1;
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
            struct Finger *f = fingers;
            for (int i=0;i< MAX_SLOTS;i++){
                if (f->state) {
                    report(f, &evt.time, i);
                    if (f->status == Down) {
                        f->status = Move;
                    } 
                    else if (f->status == Up){
                        f->state = 0;
                    }
                    
                }
                f++;
            }

        }
    }

    return 0;
}
