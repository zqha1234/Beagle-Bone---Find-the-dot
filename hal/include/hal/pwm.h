#ifndef PWM_H
#define PWM_H

#include <stdatomic.h>

// Init the PWM module for the buzzer.
void pwm_init(void);

// for the PWM hardware is properly shut down and resources are released.
void pwm_cleanup(void);

// Plays a sound sequence based on the specified event type.
void playSoundForEvent(int eventType);

#endif
