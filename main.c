#include<stdio.h>
#include<linux/input.h>


#DEFINE TOUCHSCREEN "/dev/input/event1"
#DEFINE BUTTONS "/dev/input/event2"



void emit(FILE *f, int eventType){
}

int main(){
    FILE *buttons = fopen(BUTTONS,"wb");
    if (!buttons){
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }
    FILE *touchscreen = fopen(TOUCHSCREEN,"rb");
    if (!buttons){
        fprintf(stderr, "cannot open touchscreen");
        exit(1);
    }

    struct input_event ie;

    while(fread(&ie, touchscreen)){
        printf("read event");
    }


    return 0;
}
