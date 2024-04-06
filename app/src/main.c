#include "main.h"
#include "../../hal/include/hal/accelerometer.h"
#include "../../hal/include/hal/sound.h"
#include "../../hal/include/hal/display.h"


#include "../include/sharedMem-Linux.h"
#include <stdio.h>
#include <stdbool.h>
#include "./../hal/include/hal/pwm.h"

static bool flag = true;

// terminate the program
void Program_terminate(void) {
    flag = false;
}

// return the bool value if the program should continue to run
bool isRun(void) {
    return flag; //controls the running thread
}

// wait for shutdown, and clean up the threads;
static void wait_for_shutdown(void){
    SharedMem_cleanup();
    Accelerometer_cleanup();
    pwm_cleanup();
    sound_cleanup();
    // displayCleanup();
}

int main(void) {
    Accelerometer_init();
    SharedMem_init();
    sound_init();
    // displayInit();
    wait_for_shutdown();
    return 0;
}