#ifndef PWM_H
#define PWM_H

#include <stdatomic.h>

#define PWM_EXPORT_PATH "/sys/class/pwm/pwmchip0/export"
#define PWM_UNEXPORT_PATH "/sys/class/pwm/pwmchip0/unexport"
#define PWM_PERIOD_PATH "/sys/class/pwm/pwmchip0/pwm0/period"
#define PWM_DUTY_CYCLE_PATH "/sys/class/pwm/pwmchip0/pwm0/duty_cycle"
#define PWM_ENABLE_PATH "/sys/class/pwm/pwmchip0/pwm0/enable"

// Init the PWM module for the buzzer.
void pwm_init(void);

// for the PWM hardware is properly shut down and resources are released.
void pwm_cleanup(void);

// Plays a sound sequence based on the specified event type.
void playSoundForEvent(int eventType);

void write_to_file(const char *file_path, const char *value);

#endif
