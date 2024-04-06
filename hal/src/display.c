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
#define REG_DIRA 0x02 
#define REG_DIRB 0x03 
#define REG_OUTA 0x00 
#define REG_OUTB 0x01 

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

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

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);

    if (res != 2) {
    perror("I2C: Unable to write i2c register.");
    exit(1);
}
}
static int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
    }
    return i2cFileDesc;
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
