#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../include/sharedDataStruct.h"
#include "../include/main.h"
#include "../../hal/include/hal/accelerometer.h"
#include "../../hal/include/hal/sound.h"


// General PRU Memomry Sharing Routine
// ----------------------------------------------------------------
#define PRU_ADDR      0x4A300000   // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN       0x80000      // Length of PRU memory
#define PRU0_DRAM     0x00000      // Offset to DRAM
#define PRU1_DRAM     0x02000
#define PRU_SHAREDMEM 0x10000      // Offset to shared memory
#define PRU_MEM_RESERVED 0x200     // Amount used by stack and heap

// Convert base address to each memory section
// #define PRU0_MEM_FROM_BASE(base) ( (base) + PRU0_DRAM + PRU_MEM_RESERVED)
#define PRU0_MEM_FROM_BASE(base) ((volatile sharedMemStruct_t *)((char*)(base) + PRU0_DRAM + PRU_MEM_RESERVED))
#define PRU1_MEM_FROM_BASE(base) ( (base) + PRU1_DRAM + PRU_MEM_RESERVED)
#define PRUSHARED_MEM_FROM_BASE(base) ( (base) + PRU_SHAREDMEM)


static volatile void *pPruBase;
static volatile sharedMemStruct_t *pSharedPru0;

static pthread_t sharedMem_thread; 
static int hit_count = 0;


// Return the address of the PRU's base memory
volatile void* getPruMmapAddr(void)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }

    // Points to start of PRU memory.
    volatile void* pPruBase = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);

    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase)
{
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}

static void* sharedMem_function(void* unused) {
    (void)unused;
 


    runCommand("config-pin p8.15 pruin");
    runCommand("config-pin p8.16 pruin");
    runCommand("config-pin p8.11 pruout");
    // configure pin for led displlay can add here

    // Get access to shared memory for my uses
    pPruBase = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);


    while (isRun()) {
        pSharedPru0->color_pattern = get_color_pattern();
        pSharedPru0->color = get_color();
        // printf("color pattern is %d\n", pSharedPru0->color_pattern); // debug use only
        // printf("color is %d\n", pSharedPru0->color); // debug use only
        if (pSharedPru0->color_pattern == 6 && pSharedPru0->color == 3 && pSharedPru0->isJsDown) {
            hit_count++; // led display this hit_count value
            printf("hc:\n", hit_count);
            sleepForMs(100);
            play_hit_sound();
            // printf("hit_count is: %d\n", hit_count); // debug use only
            // call sound (buzzer) function here, correct joystick press
            //play correct sound

        } else if (pSharedPru0->isJsDown) {
            // call sound (buzzer) function here, wrong joystick press 
            //play incorrect sound
            play_miss_sound();
        }
        if (pSharedPru0->isJsRight) {
            pSharedPru0->color_pattern = 12;
            // shut down the program
            Program_terminate();
        }
        sleepForMs(100);
    }

    // Cleanup
    freePruMmapAddr(pPruBase);
    return NULL;
}


// Createa a thread
void SharedMem_init(void) {
    int result_thread;
    result_thread = pthread_create(&sharedMem_thread, NULL, sharedMem_function, NULL);
    //check if the thread is created sucessfully
    if (result_thread != 0){
        // if thread cration fails, exit the program
        perror("Thread Creation Error!\n");
        exit(1);
    }
}
//to prevent race conditions with display thread we made this func
int get_hitcount(void){
    printf("hitcount: \n", hit_count);
    return hit_count;
}

// Shut down the thread
void SharedMem_cleanup(void) {
    pthread_join(sharedMem_thread, NULL);
}