#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/hal/pwm.h"

// Function prototypes
void sound_init(void);
void sound_cleanup(void);
void play_note(int frequency);
void play_hit_sound(void);
void play_miss_sound(void);

// Helper function to play a note for a duration
void play_note_for_duration(int frequency, int duration_ms);

void sound_init(void) {
    pwm_init(); // Initialize the PWM module for the buzzer
}

void sound_cleanup(void) {
    pwm_cleanup(); 
}

void play_note(int frequency) {
    if (frequency <= 0) {
        printf("Invalid frequency\n");
        return;
    }

    // Calculate period in ns (1/frequency) and set duty cycle to half
    int period_ns = 1000000000 / frequency;
    set_pwm_frequency(frequency); 
}

void play_note_for_duration(int frequency, int duration_ms) {
    play_note(frequency);
    usleep(duration_ms * 1000); // Play the note for the specified duration
    write_to_file(PWM_ENABLE_PATH, "0"); // Stop the sound
}

void play_hit_sound(void) {
    // Play C5 (523 Hz) for 200 ms as a hit sound
    play_note_for_duration(523, 200);
}

void play_miss_sound(void) {
    // Play A4 (440 Hz) for 200 ms as a miss sound
    play_note_for_duration(440, 200);
}
