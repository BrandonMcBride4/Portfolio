#include "control.h"
#include "../globals/globals.h"
#include "../tank/tank.h"
#include "../tank_bullet/tank_bullet.h"
#include "../aliens/aliens.h"
#include "../alien_bullet/alien_bullet.h"
#include "../saucer/saucer.h"
#include "../video/video.h"
#include "../image_render/image_render.h"
#include "../bunkers/bunkers.h"
#include <stdio.h>

#define GAME_OVER_LIVES 0       // 0 lives signals game over
#define RESET 0                 // Used to reset counters
#define WHITE 0xFFFFFF          // Hex color for white

#define DEFAULT_INITIALS "aaa"  // What the initials will display
#define NUM_INITIALS 3          // Number of initials for high score
#define DEFAULT_HIGH_SCORE 0    // Default high score when there is no high scores
#define HIGH_SCORE_FILE_PATH "high_scores.txt" // Name of the path for the text
#define READ "r"                // Signals read a file
#define WRITE "w+"              // Signals write and update a file

#define ASCII_A_OFFSET 65       // Ascii number for lowercase a
#define CURSOR_MAX_INDEX (NUM_INITIALS - 1) // Cursor is array of initials
#define CURSOR_MIN_INDEX 0      // Minimum cursor position
#define NUM_LETTERS_IN_ALPHABET 26  // 26 letters in alphabet
#define LETTER_POSITION_MAX_INDEX (NUM_LETTERS_IN_ALPHABET - 1) // End of letters
#define LETTER_POSITION_MIN_INDEX 0 // Starting position for letters
#define NUM_TICKS_TO_BLINK 40       // Blinks every 400 ms

#define BTN_RIGHT 1 // Mask that corresponds to button zero
#define BTN_LEFT 2  // Mask that corresponds to button one
#define BTN_DOWN 4  // Mask that corresponds to button two
#define BTN_UP 8  // Mask that corresponds to button three

static struct HighScore highScores[CONTROL_NUM_HIGH_SCORES]; // The highscores array, initials and score

static bool isPressedButtonRight = false;   // Checks if the right button is pressed
static bool isPressedButtonLeft = false;    // Checks if the left button is pressed
static bool isPressedButtonDown = false;    // Checks if the down button is pressed
static bool isPressedButtonUp = false;      // Checks if the up button is pressed
static bool isReleasedButtonRight = false;  // Checks if the right button has been released
static bool isReleasedButtonLeft = false;   // Checks if the left button has been released
static bool isReleasedButtonDown = false;   // Checks if the down button has been released
static bool isReleasedButtonUp = false;     // Checks if the up button has been released

static int32_t cursorPosition;              // Which initial is blinking
static int8_t letterPosition[NUM_INITIALS]; // Which letter is showing
static char initials[NUM_INITIALS] = {ASCII_A_OFFSET, ASCII_A_OFFSET, ASCII_A_OFFSET}; // Initials
static bool isDoneEnteringName = false;     // Done entering the name
static uint32_t cursorMarkerCounter = 0;    // Counter used to time the blinking
static bool isBlank = false;                // Is the initial blank?

// States of the state machine
static enum control_st_t
{
    init_st, //Starting state
    play_st, // Plays the game
    enter_name_st, // If you have a high score it allows you to enter name
    show_high_scores_st // Shows high scores
} currentState;

// Disables state machines for the game
void enable_game()
{
    tank_enable();          // Enable tank
    tank_bullet_enable();   // Enable tank bullet
    aliens_enable();        // Enable aliens
    alien_bullet_enable();  // Enable alien bullet
    saucer_enable();        // Enable saucer
}

// Disables state machines for the game
void disable_game()
{
    tank_disable();         // Disable tank
    tank_bullet_disable();  // Disable tank bullet
    aliens_disable();       // Disable aliens
    alien_bullet_disable(); // Disable alien bullet
    saucer_disable();       // Disable saucer
}

// Used in sorting the high scores
void swap(uint8_t j, uint8_t iMax)
{             //basic swap function
    struct HighScore temp;                                //swaps specific values in the power array
    temp = highScores[j];
    highScores[j] = highScores[iMax];         //uses a temp value to not overwrite a value
    highScores[iMax] = temp;
}

// Sorts the high scores so the highest is shown on top
void sortHighScores()
{                             //basic selection sort
    /* a[0] to a[n-1] is the array to sort */
    uint8_t i, j;
    uint8_t n = CONTROL_NUM_HIGH_SCORES;

    /* advance the position through the entire array */
    /*   (could do j < n-1 because single element is also min element) */
    for (j = 0; j < n - 1; j++)
    {
        /* find the min element in the unsorted a[j .. n-1] */

        /* assume the min is the first element */
        uint8_t iMin = j;
        /* test against elements after j to find the smallest */
        for (i = j + 1; i < n; i++)
        {
            /* if this element is greater, then it is the new maximum */
            if (highScores[i].score > highScores[iMin].score)
            {
                /* found new minimum; remember its index */
                iMin = i;
            }
        }

        if (iMin != j)
        {
            swap(j, iMin); //swaps the two elements, systematically sorting through the array
        }
    }
}

// Initialize high scores array
void readScores(FILE *fp)
{
    rewind(fp); // Put fp to the top of the file
    for (uint32_t i = 0; i < CONTROL_NUM_HIGH_SCORES; ++i)
    {
        char initials[CONTROL_INITIALS_STR_LEN]; // temp initials
        uint32_t score; // temp score

        fscanf(fp, "%s", initials); // scan for the initials
        fscanf(fp, "%d", &score);  // scan for the score

        // Copy initials into static array
        for (int j = 0; j < CONTROL_INITIALS_STR_LEN; ++j)
        {
            highScores[i].initials[j] = initials[j];
        }

        // Copy score into
        highScores[i].score = score;
    }
    sortHighScores();
}

// Write array to high scores file
void saveScores()
{
    FILE *fp_save = fopen(HIGH_SCORE_FILE_PATH, WRITE); // Open file
    for (uint32_t i = 0; i < CONTROL_NUM_HIGH_SCORES; ++i)
    {
        fprintf(fp_save, "%s %d\n", highScores[i].initials, highScores[i].score); // Write the initials and scores to file
    }
    fclose(fp_save); // Close file so it saves
}

void initHighScores()
{
    FILE *fp_read = fopen(HIGH_SCORE_FILE_PATH, READ);
    // Check if high scores file exists
    if (fp_read)
    {
        readScores(fp_read); // Initialize high score array
        fclose(fp_read);
    }
    else // If not create it with default scores of aaa 00000
    {
        FILE *fp_write = fopen(HIGH_SCORE_FILE_PATH, WRITE);
        for (uint32_t i = 0; i < CONTROL_NUM_HIGH_SCORES; ++i)
        {
            fprintf(fp_write, "%s %d\n", DEFAULT_INITIALS, DEFAULT_HIGH_SCORE); // Write in the default
        }
        readScores(fp_write); // Initialize high score array
        fclose(fp_write);
        saveScores();           // Save file
    }

}

// Initialize control state machine
void control_init()
{
    currentState = init_st;
}

// Detects if a button has released based on if it has been pressed
void control_detectRelease(bool *isButtonPressed, bool *isButtonReleased, uint32_t btnMask)
{
    uint32_t btnValue = globals_getBtnValue(); // Detect which button
    // If the button was released, then it's not anymore
    if (isButtonReleased)
    {
        *isButtonReleased = false;
    }
    // Detect if the button is currently being pressed
    if (btnValue & btnMask)
    {
        *isButtonPressed = true;
    }
    else
    {
        // If the button is not being currently pressed and
        // it was pressed, then it has just been released
        if (*isButtonPressed)
        {
            *isButtonReleased = true;
        }
        // Button is no longer being pressed
        *isButtonPressed = false;
    }

}

// Initialize the game, used in the beginning and returning from high scores
void startGame()
{
    globals_setScore(GLOBALS_STARTING_SCORE); // reset score

    globals_setNumLives(GLOBALS_STARTING_NUM_LIVES); // reset lives
    // Show initial game set up

    image_render_init();

    // Initialize tank state machine
    tank_init();

    // Initialize tank bullet state machine
    tank_bullet_init();

    // Initialize saucer state machine
    saucer_init();

    // Initialize aliens state machine
    aliens_init();

    // Initialize alien bullet state machine
    alien_bullet_init();

    // Initialize tank bullet state machine
    tank_bullet_init();

    // Initialize bunkers
    bunkers_init();

    // Allow the game to play
    enable_game();

    // Read in the values from the txt file to the array
    initHighScores();
}

// Stop the game and display game over
void enterGameOver()
{
    disable_game(); // Game shouldn't be playing in game over
    video_writeScreen(IMAGE_RENDER_BACKGROUND_COLOR); // Rewrite screen
    image_render_gameOver(); // Show game over
}

// If there was a button release perform action pertaining to the button
void processButtonRelease()
{
    // The right button moves the cursor to the right
    if (isReleasedButtonRight)
    {
        cursorPosition++;
        // If the cursor button is pushed off the screen
        // Name is entered and show
        if (cursorPosition > CURSOR_MAX_INDEX)
        {
            cursorPosition = CURSOR_MAX_INDEX;
            isDoneEnteringName = true;
        }
    }
    // Left button moves the cursor to the left
    if (isReleasedButtonLeft)
    {
        cursorPosition--;
        if (cursorPosition < CURSOR_MIN_INDEX)
            cursorPosition = CURSOR_MIN_INDEX;
    }
    // Down button moves through the alphabet
    if (isReleasedButtonDown)
    {
        letterPosition[cursorPosition]++;
        // If you reach passed z then go back to a
        if (letterPosition[cursorPosition] > LETTER_POSITION_MAX_INDEX)
        {
            letterPosition[cursorPosition] = LETTER_POSITION_MIN_INDEX;
        }
    }
    // Up button moves through the alphabet in reverse
    if (isReleasedButtonUp)
    {
        letterPosition[cursorPosition]--;
        // If you reach passed a go to z
        if (letterPosition[cursorPosition] < LETTER_POSITION_MIN_INDEX)
        {
            letterPosition[cursorPosition] = LETTER_POSITION_MAX_INDEX;
        }
    }
}

// Blinks the cursor
void blinkCursor()
{
    cursorMarkerCounter++; // Used to detect if we should blink or unblink
    if (cursorMarkerCounter == NUM_TICKS_TO_BLINK)
    {
        cursorMarkerCounter = RESET; // Reset counter
        isBlank = !isBlank; // Blink or show initial
        if (isBlank)
        {
            char tempInitials[NUM_INITIALS];
            // Get current initials
            for (int i = 0; i < NUM_INITIALS; ++i)
            {
                tempInitials[i] = initials[i];
            }

            tempInitials[cursorPosition] = ' '; // Override the cursor position with a space

            image_render_enterName(WHITE, tempInitials); // Display temporary initials
        }
        else
        {
            image_render_enterName(WHITE, initials); // If we aren't blinking show actual initials
        }
    }
}

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void debugStatePrint()
{
    static enum control_st_t previousState;
    static bool firstPass = true;
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state name over and over.
    if (previousState != currentState || firstPass)
    {
        firstPass = false;                // previousState will be defined, firstPass is false.
        previousState = currentState;     // keep track of the last state that you were in.
        switch (currentState)
        {            // This prints messages based upon the state that you were in.
            case init_st:
                printf("init_st\n");
                break;
            case play_st:
                printf("play_st\n");
                break;
            case enter_name_st:
                printf("enter_name_st\n");
                break;
            case show_high_scores_st:
                printf("show_high_scores_st\n");
                break;
        }
    }
}

void control_tick()
{
    debugStatePrint();
    //state update
    switch (currentState)
    {
        case init_st: //Starting state
            currentState = play_st; // Go to play state
            startGame();            // Enable state machines used for gameplay
            cursorPosition = RESET; // Reset cursor position
            break;
        case play_st:
            // Game over is when we run out of lives
            if (globals_getNumLives() == GAME_OVER_LIVES)
            {
                enterGameOver(); // Start game over screen
                // Check if player has a high score
                if (globals_getScore() > highScores[CONTROL_NUM_HIGH_SCORES - 1].score)
                {
                    currentState = enter_name_st;               // Go to enter name
                    image_render_enterName(WHITE, initials);    // Show default initials
                    isDoneEnteringName = false;                 // Just started entering name
                }
                // If player doesn't have a high score, don't enter his name
                else
                {
                    currentState = show_high_scores_st; // Show high scores state
                    image_render_highScores(highScores);// display high scores
                }
            }
            break;
        case enter_name_st:
            // If done entering name, show high scores
            if (isDoneEnteringName)
            {
                currentState = show_high_scores_st;
                highScores[CONTROL_NUM_HIGH_SCORES-1].score = globals_getScore(); // Replace lowest score (sorted already)
                for (int i = 0; i < NUM_INITIALS; ++i)
                {
                    highScores[CONTROL_NUM_HIGH_SCORES-1].initials[i] = initials[i]; // Write initials
                }

                sortHighScores(); // Resort high scores with your new high score
                saveScores();   // Save high score to file
                image_render_highScores(highScores); // Display the scores
            }
            break;
        case show_high_scores_st:
            // If right button is pressed start the game again
            if (isReleasedButtonRight)
            {
                currentState = init_st;
            }
            break;
        default:
            printf("control_tick state update hit default");
            break;
    }

    //state action
    switch (currentState)
    {
        case init_st: // Starting state
            break;
        case play_st: // Play state
            break;
        case enter_name_st: // Enter name
            control_detectRelease(&isPressedButtonRight, &isReleasedButtonRight, BTN_RIGHT); // Detect if right button is released
            control_detectRelease(&isPressedButtonLeft, &isReleasedButtonLeft, BTN_LEFT);   // Detect if left button is released
            control_detectRelease(&isPressedButtonDown, &isReleasedButtonDown, BTN_DOWN);   // Detect if down button is released
            control_detectRelease(&isPressedButtonUp, &isReleasedButtonUp, BTN_UP);         // Detect if up button is released
            processButtonRelease(); // Process the releases

            initials[cursorPosition] = (char)(letterPosition[cursorPosition] + ASCII_A_OFFSET); // Get the letters

            blinkCursor();  // Blink the cursor
            break;
        case show_high_scores_st:
            control_detectRelease(&isPressedButtonRight, &isReleasedButtonRight, BTN_RIGHT); // Detect if it should start the game again
            break;
        default:
            printf("control_tick state update hit default");
            break;
    }
}
