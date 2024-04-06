#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <stdint.h>

// the accelerometer.c program receives the input from acceleration info (by reading the i2c registers), 
// and according to the acceleration, it will control the beat-box to play certain sounds 
// (base_drum, snare, hi_hat). 

// define the i2c
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
// define i2c device address
#define I2C_DEVICE_ADDRESS_ 0x18
// I2C register for accelerometer
#define CTRL_REG1 0x20
#define CTRL_REG4 0x23
// I2C registers for x y z
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
// I2C register for status
#define STATUS_REG 0x27

// create the thread
void Accelerometer_init(void);
// clean up the thread
void Accelerometer_cleanup(void);
// delay for xxxx ms
void sleepForMs(long long delayInMs);
// run linux command
void runCommand(char* command);
// generate random numbers
void set_random(void);

uint8_t get_color_pattern();
uint8_t get_color();

#endif