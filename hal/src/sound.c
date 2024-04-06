#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/hal/pwm.h"

// Define paths according to your system's PWM interface
#define PWM_ENABLE_PATH "/dev/bone/pwm/0/a/enable"
#define PWM_PERIOD_PATH "/dev/bone/pwm/0/a/period"
#define PWM_DUTY_CYCLE_PATH "/dev/bone/pwm/0/a/duty_cycle"

// Prototypes for internal functions
// int write_to_file(const char *file_path, const char *value);

// Public function prototypes
void sound_init(void);
void sound_cleanup(void);
void play_note(int frequency);
void play_note_for_duration(int frequency, int duration_ms);
void play_hit_sound(void);
void play_miss_sound(void);

void sound_init(void) {
    runCommand("config-pin p9.22 pwm");
}

void sound_cleanup(void) {
    // Silence the PWM output
    write_to_file(PWM_ENABLE_PATH, "0");
}

void play_note(int frequency) {
    if (frequency <= 0) {
        printf("Invalid frequency\n");
        return;
    }

    // Enable PWM output
    write_to_file(PWM_ENABLE_PATH, "1");

    // Calculate period (ns) and duty cycle (50% of the period) for the desired frequency
    long period_ns = 1000000000 / frequency;
    char period_str[32];
    snprintf(period_str, sizeof(period_str), "%ld", period_ns);
    write_to_file(PWM_PERIOD_PATH, period_str);

    // Set duty cycle to 50% of the period
    snprintf(period_str, sizeof(period_str), "%ld", period_ns / 2);
    write_to_file(PWM_DUTY_CYCLE_PATH, period_str);
}

void play_note_for_duration(int frequency, int duration_ms) {
    play_note(frequency);
    usleep(duration_ms * 1000); // Play the note for the specified duration
    write_to_file(PWM_ENABLE_PATH, "0"); // Silence the PWM output after playing the note
}

void play_hit_sound(void) {
    play_note_for_duration(523, 200); // Play C5 (523 Hz) for 200 ms as a hit sound
}

void play_miss_sound(void) {
    play_note_for_duration(440, 200); // Play A4 (440 Hz) for 200 ms as a miss sound
}

// Internal helper function to write a string value to a file
// int write_to_file(const char *file_path, const char *value) {
//     FILE *fp = fopen(file_path, "w");
//     if (!fp) {
//         perror("Error opening file");
//         return -1;
//     }
//     fprintf(fp, "%s", value);
//     fclose(fp);
//     return 0;
// }
