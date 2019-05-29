#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "version.h"
#include "gestures.h"
#include "eventreader.h"
#include "keyinjector.h"



int main() {
    printf("touchinjector %s\n",VERSION);

    injector_init(); 
	ui_init();

    process_touch(&recognize_gestures);

    return 0;
}
