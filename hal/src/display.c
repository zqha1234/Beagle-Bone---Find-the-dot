#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <stdatomic.h>  // For atomic_bool
#include "../include/hal/display.h"
#include "../../app/include/main.h"
#include "../../app/include/sharedMem-Linux.h"

#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00  // Set direction of bank A pins
#define REG_DIRB 0x01  // Set direction of bank B pins
#define REG_OUTA 0x02  // Output register for bank A
#define REG_OUTB 0x03  // Output register for bank B

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

int i2cFileDesc;
atomic_bool keep_display_running = true;

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
    while (atomic_load(&keep_display_running)) {
        int current_hit_count = atomic_load(&hit_count);  // Ensure get_hitCount() is properly synchronized
        printf("received hc: %d \n", current_hit_count);
        displayScore(current_hit_count);
        usleep(1000000); // Refresh every second, using usleep for microsecond precision
    }
    return NULL;
}

void displayInit(void) {
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00); // Set all pins of bank A to output
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00); // Set all pins of bank B to output
    
    // Initialize the atomic flag
    atomic_store(&keep_display_running, true);

    if (pthread_create(&display_thread, NULL, display_update_thread, NULL) != 0) {
        perror("Failed to create the display update thread");
        exit(1);
    }
}

void displayScore(int score) {
    unsigned int d1 = score / 10; // Tens digit
    unsigned int d2 = score % 10; // Ones digit

    // Ensure that the display is not trying to show more than two digits
    d1 = d1 % 10;
    d2 = d2 % 10;

    writeI2cReg(i2cFileDesc, REG_OUTA, segment_digits[d1][0]); // Tens digit segments
    writeI2cReg(i2cFileDesc, REG_OUTB, segment_digits[d2][1]); // Ones digit segments
}

void displayCleanup(void) {
    atomic_store(&keep_display_running, false);
    pthread_join(display_thread, NULL);
    
    writeI2cReg(i2cFileDesc, REG_OUTA, 0xFF); // Turn off display
    writeI2cReg(i2cFileDesc, REG_OUTB, 0xFF);
    close(i2cFileDesc);
}
