#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "ui.h"
#include "keyinjector.h"


bool touch_enabled = false;
int f;
void injector_init() {
    f = open(BUTTONS,O_WRONLY);
    if (!f){
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }
}

void myemit(int code){
    if (!touch_enabled){
        return;
    }
    struct input_event key_input_event;
    memset(&key_input_event, 0, sizeof(key_input_event));

    debug_print("emitting %d \n", code);

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
