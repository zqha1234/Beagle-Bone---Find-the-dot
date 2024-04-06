#include "../../app/include/main.h"
#include "../../app/include/sharedMem-Linux.h"
#include "../include/hal/display.h"
#include "../include/hal/accelerometer.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <time.h>

static int i2cFileDesc;
static pthread_t background_thread; 
static int display_value = 0;
static char left_display[10] = {0xD0, 0xC0, 0x98, 0xD8, 0x0A, 0x58, 0x58, 0x02, 0xD8, 0xD8}; 
static char right_display[10] = {0xE1, 0x00, 0x83, 0x03, 0x2A, 0x23, 0xA3, 0x05, 0xA3, 0x23}; 
static int left_value;
static int right_value;


// Write the file 
void writeFile (char* fileName, char* value) {
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL) {
        printf("ERROR: Unable to open %s.\n", fileName);
        exit(1);
    }

    int charWritten = fprintf(pFile, value);
    if (charWritten <= 0) {
        printf("ERROR: Cannot write %s.\n", fileName);
        exit(1);
    }

    fclose(pFile);
}

// Initialize the I2C device
static int initI2cBus(char* bus, int address) { 
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

// Write an I2C device's register
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

// Initianize the I2c setup
void initI2c(void) {
    // (bbg)$ config-pin P9_18 i2c
    // (bbg)$ config-pin P9_17 i2c
    runCommand("sudo config-pin P9_18 i2c");
    runCommand("sudo config-pin P9_17 i2c"); 
    writeFile(GPIO_EXPORT, "61");
    writeFile(GPIO_EXPORT, "44");
    writeFile(GPIO61_DIR, "out");
    writeFile(GPIO44_DIR, "out");
    writeFile(GPIO61_VAL, "1");
    writeFile(GPIO44_VAL, "1");
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
}

// Display the value on fourteen segmentation display
static void *display_function(void* unused) {
    (void)unused;
    while (isRun()) {
        // // the maximum display_value is 99
        // if (display_value > 99) {
        //     display_value = 99;
        // } 
        display_value = get_hit_count();
        left_value = display_value / 10;
        right_value = display_value % 10;
        writeFile(GPIO61_VAL, "0");
        writeFile(GPIO44_VAL, "0");
        writeI2cReg(i2cFileDesc, REG_OUTA, left_display[left_value]);
        writeI2cReg(i2cFileDesc, REG_OUTB, right_display[left_value]);
        writeFile(GPIO61_VAL, "1");
        sleepForMs(5); // sleep for 5ms
        writeFile(GPIO61_VAL, "0");
        writeFile(GPIO44_VAL, "0");
        writeI2cReg(i2cFileDesc, REG_OUTA, left_display[right_value]);
        writeI2cReg(i2cFileDesc, REG_OUTB, right_display[right_value]);
        writeFile(GPIO44_VAL, "1");
        sleepForMs(5); // sleep for 5ms
    }
    return NULL;
}

// Createa a background thread
void Display_init(void) {
    initI2c();
    int result_thread;
    result_thread = pthread_create(&background_thread, NULL, display_function, NULL);
    //check if the thread is created sucessfully
    if (result_thread != 0){
        // if thread cration fails, exit the program
        perror("Thread Creation Error!\n");
        exit(1);
    }
}


// Shut down thread, and close i2cFileDesc
void Display_cleanup(void) {
    pthread_join(background_thread, NULL);
    writeFile(GPIO61_VAL, "0");
    writeFile(GPIO44_VAL, "0");
    close(i2cFileDesc);
}
