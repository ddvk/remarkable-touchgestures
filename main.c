#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "version.h"
#include "gestures.h"
#include "eventreader.h"
#include "keyinjector.h"


bool verbose = false;
int main() {
    printf("touchinjector %s\n",VERSION);
    //todo: option parsing
    verbose = true; 

    injector_init(); 
#ifdef REMARKABLE1
	ui_init();
#endif

    process_touch(&recognize_gestures);

    return 0;
}
