#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/hal/pwm.h"

// Paths for the PWM in the sysfs interface
#define PWM_EXPORT_PATH "/sys/class/pwm/pwmchip0/export"
#define PWM_UNEXPORT_PATH "/sys/class/pwm/pwmchip0/unexport"
#define PWM_PERIOD_PATH "/sys/class/pwm/pwmchip0/pwm0/period"
#define PWM_DUTY_CYCLE_PATH "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define PWM_ENABLE_PATH "/sys/class/pwm/pwmchip0/pwm0/enable"

// Function to write a value to a given file path
void write_to_file(const char *file_path, const char *value) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s", value);
    fclose(fp);
}

// Exports the PWM device for use
static void export_pwm() {
    write_to_file(PWM_EXPORT_PATH, "0"); 
}

// Unexports the PWM device, releasing it
static void unexport_pwm() {
    write_to_file(PWM_UNEXPORT_PATH, "0");
}

// Initializes the PWM device
void pwm_init(void) {
    export_pwm(); // Make PWM device available
    write_to_file(PWM_PERIOD_PATH, "20000000"); // period = 20ms 
    write_to_file(PWM_DUTY_CYCLE_PATH, "1500000"); // Set duty cycle to 1.5ms --> neutral
    write_to_file(PWM_ENABLE_PATH, "1"); // Enable PWM output
}

//cleaning
void pwm_cleanup(void) {
    write_to_file(PWM_ENABLE_PATH, "0"); // Disable PWM output
    unexport_pwm(); // Release PWM device
}

// Sets the PWM frequency
void set_pwm_frequency(int frequency) {
    // Frequency to period conversion (in nanoseconds)
    long period_ns = 1000000000 / frequency;
    char period_str[20];
    snprintf(period_str, sizeof(period_str), "%ld", period_ns);
    write_to_file(PWM_PERIOD_PATH, period_str);
    snprintf(period_str, sizeof(period_str), "%ld", period_ns / 2);
    write_to_file(PWM_DUTY_CYCLE_PATH, period_str);
}

