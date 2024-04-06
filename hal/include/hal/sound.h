#ifndef SOUND_H
#define SOUND_H

void sound_init(void);
void sound_cleanup(void);
void play_note(int frequency);
void play_note_for_duration(int frequency, int duration_ms);
void play_hit_sound(void);
void play_miss_sound(void);
void Buzzer_init(void);
void Buzzer_cleanup(void);
void set_hit_sound(void);
void set_miss_sound(void);

#endif 
