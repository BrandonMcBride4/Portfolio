#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#define CONTROL_INITIALS_STR_LEN 3 // Number of initials to enter in
#define CONTROL_NUM_HIGH_SCORES 10 // Number of high scores to show

// A high score contains initials and a score
struct HighScore
{
    char initials[CONTROL_INITIALS_STR_LEN]; // Initials of a high score
    uint32_t score;                          // The high score
};

// Initialize control state machine
void control_init();

// Tick of the state machine
void control_tick();

void control_detectRelease(bool *isButtonPressed, bool *isButtonReleased, uint32_t btnMask);

#endif // CONTROL_H