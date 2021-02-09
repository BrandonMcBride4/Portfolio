#ifndef USER_AUDIO_H
#define USER_AUDIO_H

#include <stdint.h>
#include <stdbool.h>

// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t user_audio_init();

// True if audio is currently being played
bool user_audio_isPlaying();

// close the UIO device
// free the wav file pointers
void user_audio_exit();

// plays the invader_die.wav file
void user_audio_playInvaderDie();

// plays the laser.wav file
void user_audio_playLaser();

// plays the player_die.wav file
void user_audio_playPlayerDie();

// set isUFOPlaying flag to true
void user_audio_enableUFOSound();

// sets isUFOPlaying flag to false
void user_audio_disableUFOSound();

// plays the ufo.wav file
void user_audio_playUFO();

// plays the ufo_die.wav file
void user_audio_playUFODie();

// plays one of the walk.wav files
void user_audio_playMarch(uint32_t walk);

// Turns volume up
void user_audio_volumeUp();

// Turns volume down
void user_audio_volumeDown();


#endif