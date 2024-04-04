#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include "../include/hal/display.h"

#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x02
#define REG_OUTB 0x03
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

int i2cFileDesc;

// Segment encoding for numbers 0-9
static const unsigned char segment_digits[10][2] = {
    {0xd0, 0xe1}, // 0
    {0xc0, 0x00}, // 1
    {0x98, 0xc3}, // 2
    {0xd8, 0x43}, // 3
    {0xc8, 0x22}, // 4
    {0x58, 0x63}, // 5
    {0x58, 0xe3}, // 6
    {0xc0, 0x61}, // 7
    {0xd8, 0xe3}, // 8
    {0xd8, 0x63}  // 9
};

static pthread_t display_thread;
static volatile int keep_display_running = 1;
// extern volatile int hit_count; no longer need this

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
    unsigned char buff[2] = {regAddr, value};
    if (write(i2cFileDesc, buff, 2) != 2) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
}

static int initI2cBus(char* bus, int address) {
    int file = open(bus, O_RDWR);
    if (file < 0) {
        perror("I2C: Unable to open bus for read/write");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, address) < 0) {
        perror("I2C: Failed to connect to the device.");
        close(file);
        exit(1);
    }
    return file;
}

static void* display_update_thread(void* args) {
    while (keep_display_running) {
        int display_value = get_hitCount();
        displayScore(display_value);
        sleep(1); // Refresh every second
    }
    return NULL;
}

void displayInit(void) {
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00); // Set all pins of bank A to output
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00); // Set all pins of bank B to output
    
    // Start the display update thread
    if (pthread_create(&display_thread, NULL, display_update_thread, NULL) != 0) {
        perror("Failed to create the display update thread");
        exit(1);
    }
}

void displayScore(int score) {
    unsigned int d1 = score / 10; // Tens digit
    unsigned int d2 = score % 10; // Ones digit

    writeI2cReg(i2cFileDesc, REG_OUTA, segment_digits[d1][0]); // Tens digit segments
    writeI2cReg(i2cFileDesc, REG_OUTB, segment_digits[d2][0]); // Ones digit segments
}

void displayCleanup(void) {
    keep_display_running = 0;
    pthread_join(display_thread, NULL);
    
    writeI2cReg(i2cFileDesc, REG_OUTA, 0xFF); // Turn off display
    writeI2cReg(i2cFileDesc, REG_OUTB, 0xFF);
    close(i2cFileDesc);
}
