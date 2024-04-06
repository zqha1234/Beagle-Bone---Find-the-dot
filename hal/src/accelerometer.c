#include <pthread.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "../include/hal/accelerometer.h"
#include "../../app/include/main.h"

static pthread_t acc_thread; 
static int i2cFileDesc;
static int16_t x;
static int16_t y;
static uint8_t color_pixel_pattern;
static uint8_t color_pixel;
static unsigned char xl = OUT_X_L;
static unsigned char xh = OUT_X_H;
static unsigned char yl = OUT_Y_L;
static unsigned char yh = OUT_Y_H;
static double one_g = 16384.0;

static int x_random = 0;
static int y_random = 0;

// Delay in ms
void sleepForMs(long long delayInMs) {
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

// Run a Linux command
void runCommand(char* command) {
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it 
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
            // printf("--> %s", buffer); // Uncomment for debugging
        }
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
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


// Read from I2C device's registers
static unsigned char* readI2cReg(int i2cFileDesc, unsigned char regAddr) {
    // To read a register, must first write the address
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    unsigned char *buff = malloc(sizeof(char)*4);
    write(i2cFileDesc, &xl, 1);
    char data[1] = {0};
    read(i2cFileDesc, data, 1);
    buff[0] = data[0];
    write(i2cFileDesc, &xh, 1);
    read(i2cFileDesc, data, 1);
    buff[1] = data[0];
    write(i2cFileDesc, &yl, 1);
    read(i2cFileDesc, data, 1);
    buff[2] = data[0];
    write(i2cFileDesc, &yh, 1);
    read(i2cFileDesc, data, 1);
    buff[3] = data[0];
    return buff;
}

void set_random(void) {
    srand(time(NULL));
    x_random = (rand() % 11) - 5;
    srand(time(NULL));
    y_random = (rand() % 11) - 5;
}

// initialize the accelometer
static void accInit(void) {
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS_);
    writeI2cReg(i2cFileDesc, CTRL_REG1, 0x27);
    writeI2cReg(i2cFileDesc, CTRL_REG4, 0x00);
}

uint8_t get_color_pattern() {
    return color_pixel_pattern;
}

uint8_t get_color() {
    return color_pixel;
}

static void* acc_function(void* unused) {
    (void)unused;
    accInit();
    set_random();
    while (isRun()) {
        unsigned char *values = readI2cReg(i2cFileDesc, STATUS_REG);
        x = ((values[1] << 8) | values[0]);
        y = ((values[3] << 8) | values[2]);
        // printf("x: %f\n", (y/ one_g)); // debug use
        if (((y / one_g) - y_random / 10.0) <= -0.5) {
            color_pixel_pattern = 1;
        } else if (((y / one_g) - y_random / 10.0) >= -0.5 && ((y / one_g) - y_random / 10.0) < -0.4) {
            color_pixel_pattern = 2;
        } else if (((y / one_g) - y_random / 10.0) >= -0.4 && ((y / one_g) - y_random / 10.0) < -0.3) {
            color_pixel_pattern = 3;
        } else if (((y / one_g) - y_random / 10.0) >= -0.3 && ((y / one_g) - y_random / 10.0) < -0.2) {
            color_pixel_pattern = 4;
        } else if (((y / one_g) - y_random / 10.0) >= -0.2 && ((y / one_g) - y_random / 10.0) < -0.1) {
            color_pixel_pattern = 5;
        } else if (((y / one_g) - y_random / 10.0) >= -0.1 && ((y / one_g) - y_random / 10.0) < 0.1) {
            color_pixel_pattern = 6;
        } else if (((y / one_g) - y_random / 10.0) >= 0.1 && ((y / one_g) - y_random / 10.0) < 0.2) {
            color_pixel_pattern = 7;
        } else if (((y / one_g) - y_random / 10.0) >= 0.2 && ((y / one_g) - y_random / 10.0) < 0.3) {
            color_pixel_pattern = 8;
        } else if (((y / one_g) - y_random / 10.0) >= 0.3 && ((y / one_g) - y_random / 10.0) < 0.4) {
            color_pixel_pattern = 9;
        } else if (((y / one_g) - y_random / 10.0) >= 0.4 && ((y / one_g) - y_random / 10.0) < 0.5) {
            color_pixel_pattern = 10;
        } else if (((y / one_g) - y_random / 10.0) >= 0.5) {
            color_pixel_pattern = 11;
        }
        if (((x / one_g) - x_random / 10.0) < -0.1) {
            color_pixel = 1;
        } else if (((x / one_g) - x_random / 10.0) > 0.1) {
            color_pixel = 2;
        } else {
            color_pixel = 3;
        }
        free(values);
        values = NULL;
        sleepForMs(100);
    }
    return NULL;
}

// Createa a thread
void Accelerometer_init(void) {
    int result_thread;
    result_thread = pthread_create(&acc_thread, NULL, acc_function, NULL);
    //check if the thread is created sucessfully
    if (result_thread != 0){
        // if thread cration fails, exit the program
        perror("Thread Creation Error!\n");
        exit(1);
    }
}

// Shut down the thread
void Accelerometer_cleanup(void) {
    pthread_join(acc_thread, NULL);
    writeI2cReg(i2cFileDesc, CTRL_REG1, 0x07);
    close(i2cFileDesc);
}

