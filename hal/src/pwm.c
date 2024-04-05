#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/hal/pwm.h"

#define PWM_EXPORT_PATH "/sys/class/pwm/pwmchip0/export"
#define PWM_UNEXPORT_PATH "/sys/class/pwm/pwmchip0/unexport"
#define PWM_PERIOD_PATH "/dev/bone/pwm/0/a/period"
#define PWM_DUTY_CYCLE_PATH "/dev/bone/pwm/0/a/duty_cycle"
#define PWM_ENABLE_PATH "/dev/bone/pwm/0/a/enable"

// Function to write a value to a given file path, returns 0 on success, -1 on failure
int write_to_file(const char *file_path, const char *value) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }
    fprintf(fp, "%s", value);
    fclose(fp);
    return 0;
}

// Exports the PWM device for use, returns 0 on success, -1 on failure
static int export_pwm() {
    return write_to_file(PWM_EXPORT_PATH, "0"); 
}

// Unexports the PWM device, releasing it, returns 0 on success, -1 on failure
static int unexport_pwm() {
    return write_to_file(PWM_UNEXPORT_PATH, "0");
}

//i might need to add error handling
void pwm_init(void) {
    // Attempt to export the PWM channel.......handle failure if occurs
    if (export_pwm() == -1) {
        fprintf(stderr, "Failed to export PWM..permissions\n");
        exit(EXIT_FAILURE); 
    }

    // Set the PWM period. On failure, clean up and exit.
    if (write_to_file(PWM_PERIOD_PATH, "20000000") == -1) { // period = 20ms
        fprintf(stderr, "Failed to set PWM period\n");
        unexport_pwm(); // Attempt to clean up by unexporting the PWM channel
        exit(EXIT_FAILURE);
    }

    // Set the PWM duty cycle. On failure, clean up and exit.
    if (write_to_file(PWM_DUTY_CYCLE_PATH, "1500000") == -1) { // duty cycle = 1.5ms --> neutral position
        fprintf(stderr, "Failed to set PWM duty cycle\n");
        unexport_pwm(); // Attempt to clean up by unexporting the PWM channel
        exit(EXIT_FAILURE);
    }

    // Enable the PWM output. On failure, clean up and exit.
    if (write_to_file(PWM_ENABLE_PATH, "1") == -1) { // Enable PWM output
        fprintf(stderr, "Failed to enable PWM output\n");
        unexport_pwm(); // Clean up by unexporting the PWM channel
        exit(EXIT_FAILURE);
    }
}
// Cleans up the PWM by disabling it and unexporting, consider adding error handling
void pwm_cleanup(void) {
    write_to_file(PWM_ENABLE_PATH, "0"); // Disable PWM output
    unexport_pwm(); // Consider checking return value
}

// Sets the PWM frequency, consider adding error handling
void set_pwm_frequency(int frequency) {
    long period_ns = 1000000000 / frequency; // Frequency to period conversion (in nanoseconds)
    char period_str[20];
    snprintf(period_str, sizeof(period_str), "%ld", period_ns);
    write_to_file(PWM_PERIOD_PATH, period_str);
    snprintf(period_str, sizeof(period_str), "%ld", period_ns / 2);
    write_to_file(PWM_DUTY_CYCLE_PATH, period_str);
}
