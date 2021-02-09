#include "image_render.h"
#include "sprite.h"
#include "../video/video.h"
#include "../globals/globals.h"
#include <ctype.h>
#include <stdio.h>

#define SAUCER_MAGNIFY 2    // How much the saucer is magnified

#define DECIMAL 10          // Hex is 16, decimal is 10, used to get the high score

#define MAX_NUM_LIVES 5     // Maximum number of lives

#define SPRITE_OFFSET IMAGE_RENDER_SPRITE_MAGNIFY   // How much space is between letters
#define SPRITE_WHITE 0xFFFFFF                       // White
#define SPRITE_GREEN 0x00FF00                       // Green
#define SPRITE_LETTER_WIDTH (IMAGE_RENDER_SPRITE_WIDTH*IMAGE_RENDER_SPRITE_MAGNIFY+SPRITE_OFFSET)

#define BOTTOM_LINE_X 0                             // Where the bottom line x starts
#define BOTTOM_LINE_1_Y VIDEO_SCREEN_HEIGHT-1       // Where the 1st bottom line y is
#define BOTTOM_LINE_2_Y VIDEO_SCREEN_HEIGHT-2       // Where the 2nd bottom lin y is
#define BOTTOM_LINE_COLOR 0x00FF00                  // Bottom line color is green
#define BOTTOM_LINE_WIDTH VIDEO_SCREEN_WIDTH        // The line spans the whole screen

#define SCORE_STR "score"                           // Used to display score
#define SCORE_START_X 20                            // Where the score starts
#define SCORE_START_Y (IMAGE_RENDER_TANK_HEIGHT*IMAGE_RENDER_TANK_MAGNIFY - IMAGE_RENDER_SPRITE_HEIGHT*IMAGE_RENDER_SPRITE_MAGNIFY) // Score y
#define SCORE_TEN_THOUSANDS_X (SCORE_START_X+SPRITE_LETTER_WIDTH*6) // Where the numbers of the score start

#define LIVES_STR "lives"   // Used to display the word lives
#define LIVES_X (VIDEO_SCREEN_WIDTH-((IMAGE_RENDER_TANK_WIDTH*IMAGE_RENDER_TANK_MAGNIFY+SPRITE_OFFSET)*5 + SPRITE_LETTER_WIDTH*5 + SPRITE_OFFSET)) // Where lives is located
#define LIVES_Y SCORE_START_Y   // Lives is aligned with the score
#define LIFE_MARKER_Y 0         // Where life marker (the tank) y is located
#define LIFE_MARKER_X (LIVES_X+SPRITE_LETTER_WIDTH*5)   // Where the tank marker x is
#define LIFE_MARKER_X_WIDTH (IMAGE_RENDER_TANK_WIDTH*IMAGE_RENDER_TANK_MAGNIFY+SPRITE_OFFSET) // Pixel width of the tank

#define GAME_OVER_STR "game over"       // Used to display game over
#define GAME_OVER_MAGNIFY 8             // How much game over is magnified
#define GAME_OVER_START_X 100           // Where game over left coordinate is
#define GAME_OVER_START_Y 20            // Wehre game over top coordinate is
#define GAME_OVER_LETTER_WIDTH (IMAGE_RENDER_SPRITE_WIDTH*GAME_OVER_MAGNIFY+GAME_OVER_MAGNIFY)  // How wide each letter is in game over
#define GAME_OVER_LETTER_HEIGHT (IMAGE_RENDER_SPRITE_HEIGHT*GAME_OVER_MAGNIFY)                  // How high each letter is in game over

#define ENTER_NAME_STR "enter your name" // Used to display enter your name
#define NAME_X (GAME_OVER_START_X + GAME_OVER_LETTER_WIDTH*2.5)                 // Where the name coordinate to begin is
#define NAME_Y_OFFSET (GAME_OVER_MAGNIFY*IMAGE_RENDER_SPRITE_MAGNIFY)           // Pixel space between game over and enter your name
#define NAME_Y (GAME_OVER_START_Y + GAME_OVER_LETTER_HEIGHT + NAME_Y_OFFSET)    // Where the name coordinate to begin is

#define INITIALS_X (GAME_OVER_START_X + GAME_OVER_LETTER_WIDTH*4)               // The x position of initials to enter
#define ROW_OFFSET (IMAGE_RENDER_SPRITE_HEIGHT*IMAGE_RENDER_SPRITE_MAGNIFY + NAME_Y_OFFSET) // Number of pixels between each high score
#define INITIALS_Y (NAME_Y + ROW_OFFSET)    // Y coordinate used to show the initials when entering your name
#define ERASE_INITIALS "   "                // How to erase the initial


#define HIGH_SCORES_STR "high scores"       // Used to display high scores
#define HIGH_SCORES_STR_X (GAME_OVER_START_X + GAME_OVER_LETTER_WIDTH*3) // Where high score x coordinate is
#define HIGH_SCORES_STR_Y NAME_Y                                         // Y coordinate for high scores


#define HIGH_SCORES_INITIALS_START_X (HIGH_SCORES_STR_X + SPRITE_LETTER_WIDTH)  // The x position of all of the initials
#define HIGH_SCORES_INITIALS_START_Y INITIALS_Y                                 // The y position of the first row
#define HIGH_SCORES_SCORES_X_OFFSET (SPRITE_LETTER_WIDTH*(CONTROL_INITIALS_STR_LEN+1)) // Pixel space between initals and scores

// Draws an object starting at x, y line by line
void drawObject(uint32_t x, uint32_t y, uint32_t height, uint32_t width,
                uint32_t mag, uint32_t color, const uint32_t *object)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < mag; j++)
        {
            video_writeMaskedLine(x, y + j + i * mag, color, width, object[i], mag); // Draw the line of the object
        }
    }
}

// Draw a alphanumeric character green at a x, y coordinate
void drawSpriteGreen(uint32_t x, uint32_t y, const uint32_t *sprite)
{
    drawObject(x, y, IMAGE_RENDER_SPRITE_HEIGHT, IMAGE_RENDER_SPRITE_WIDTH, IMAGE_RENDER_SPRITE_MAGNIFY, SPRITE_GREEN,
               sprite); // Draw sprite
}

/* returns length of string s */
uint32_t strLen(const char *s)
{
    uint32_t i;
    for (i = 0; s[i] != '\0'; i++); // Goes until the string is done
    return i; // Return the length of string
}

// Returns the bitmask given a number
const uint32_t *getNumberSprite(const uint32_t num)
{
    switch (num)
    {
        case 0:
            return number0_5x5; // Bitmask of 0
        case 1:
            return number1_5x5; // Bitmask of 1
        case 2:
            return number2_5x5; // Bitmask of 2
        case 3:
            return number3_5x5; // Bitmask of 3
        case 4:
            return number4_5x5; // Bitmask of 4
        case 5:
            return number5_5x5; // Bitmask of 5
        case 6:
            return number6_5x5; // Bitmask of 6
        case 7:
            return number7_5x5; // Bitmask of 7
        case 8:
            return number8_5x5; // Bitmask of 8
        case 9:
            return number9_5x5; // Bitmask of 9
    }
}

// Returns a bitmask of a given character
const uint32_t *getCharacterSprite(const int c)
{
    // If letter is uppercase turn to lower
    switch (tolower(c))
    {
        case 'a':
            return letterA_5x5; // Bitmask of a
        case 'b':
            return letterB_5x5; // Bitmask of b
        case 'c':
            return letterC_5x5; // Bitmask of c
        case 'd':
            return letterD_5x5; // Bitmask of d
        case 'e':
            return letterE_5x5; // Bitmask of e
        case 'f':
            return letterF_5x5; // Bitmask of f
        case 'g':
            return letterG_5x5; // Bitmask of g
        case 'h':
            return letterH_5x5; // Bitmask of h
        case 'i':
            return letterI_5x5; // Bitmask of i
        case 'j':
            return letterJ_5x5; // Bitmask of j
        case 'k':
            return letterK_5x5; // Bitmask of k
        case 'l':
            return letterL_5x5; // Bitmask of l
        case 'm':
            return letterM_5x5; // Bitmask of m
        case 'n':
            return letterN_5x5; // Bitmask of n
        case 'o':
            return letterO_5x5; // Bitmask of o
        case 'p':
            return letterP_5x5; // Bitmask of p
        case 'q':
            return letterQ_5x5; // Bitmask of q
        case 'r':
            return letterR_5x5; // Bitmask of r
        case 's':
            return letterS_5x5; // Bitmask of s
        case 't':
            return letterT_5x5; // Bitmask of t
        case 'u':
            return letterU_5x5; // Bitmask of u
        case 'v':
            return letterV_5x5; // Bitmask of v
        case 'w':
            return letterW_5x5; // Bitmask of w
        case 'x':
            return letterX_5x5; // Bitmask of x
        case 'y':
            return letterY_5x5; // Bitmask of y
        case 'z':
            return letterZ_5x5; // Bitmask of z
        case ' ':
            return letterBLANK_5x5; // Bitmask of space
        case '0':
            return number0_5x5; // Bitmask of 0
        case '1':
            return number1_5x5; // Bitmask of 1
        case '2':
            return number2_5x5; // Bitmask of 2
        case '3':
            return number3_5x5; // Bitmask of 3
        case '4':
            return number4_5x5; // Bitmask of 4
        case '5':
            return number5_5x5; // Bitmask of 5
        case '6':
            return number6_5x5; // Bitmask of 6
        case '7':
            return number7_5x5; // Bitmask of 7
        case '8':
            return number8_5x5; // Bitmask of 8
        case '9':
            return number9_5x5; // Bitmask of 9
        default:
            return letterBLANK_5x5; // Blank by default
    }
}

// Displays a string at an x, y given a size and color
void displayString(uint32_t x, uint32_t y, uint32_t rgbValue, uint32_t mag, const char *str)
{
    uint32_t space_between_letters = mag; // The space between each letter is the magnified value
    for (int i = 0; i < strLen(str); ++i)
    {
        drawObject(x + i * (space_between_letters + IMAGE_RENDER_SPRITE_WIDTH * mag), y,
                   IMAGE_RENDER_SPRITE_HEIGHT, IMAGE_RENDER_SPRITE_WIDTH,
                   mag, rgbValue, getCharacterSprite(str[i])); // Draws each character individually
    }
}

// Render the top bar, bunkers and bottom lines
void image_render_init()
{
    video_writeScreen(IMAGE_RENDER_BACKGROUND_COLOR); // Redraw screen
    video_writeHLine(BOTTOM_LINE_X, BOTTOM_LINE_1_Y, BOTTOM_LINE_COLOR, BOTTOM_LINE_WIDTH); // Draw first line at bottom
    video_writeHLine(BOTTOM_LINE_X, BOTTOM_LINE_2_Y, BOTTOM_LINE_COLOR, BOTTOM_LINE_WIDTH); // Draw second line at bottom
    image_render_topBar();  // Draw the score and lives

    image_render_bunker(IMAGE_RENDER_BUNKER_0_X, IMAGE_RENDER_BUNKER_Y); // Draw a bunker
    image_render_bunker(IMAGE_RENDER_BUNKER_1_X, IMAGE_RENDER_BUNKER_Y); // Draw a bunker
    image_render_bunker(IMAGE_RENDER_BUNKER_2_X, IMAGE_RENDER_BUNKER_Y); // Draw a bunker
    image_render_bunker(IMAGE_RENDER_BUNKER_3_X, IMAGE_RENDER_BUNKER_Y); // Draw a bunker
}

// Render the saucer at a certain x, y coordinate (x, y is top left corner of object)
void image_render_saucer(uint32_t x, uint32_t y)
{
    drawObject(x, y, IMAGE_RENDER_SAUCER_HEIGHT, IMAGE_RENDER_SAUCER_WIDTH, SAUCER_MAGNIFY, IMAGE_RENDER_SAUCER_COLOR, saucer_18x7);
}

// Render the saucer erased at a certain x, y coordinate (x, y is top left corner of object)
void image_render_saucerGone(uint32_t x, uint32_t y)
{
    drawObject(x, y, IMAGE_RENDER_SAUCER_HEIGHT, IMAGE_RENDER_SAUCER_WIDTH, SAUCER_MAGNIFY, IMAGE_RENDER_SAUCER_COLOR,
               saucer_gone_18x7);
}

// Render an alien at a certain x, y coordinate (x, y is top left corner of object)
void image_render_singleAlien(uint32_t x, uint32_t y, const uint32_t *alien)
{
    drawObject(x, y, IMAGE_RENDER_ALIEN_HEIGHT, IMAGE_RENDER_ALIEN_WIDTH, IMAGE_RENDER_ALIEN_MAGNIFY, IMAGE_RENDER_ALIEN_COLOR,
               alien);
}

// Render the alien army at a certain x, y coordinate (x, y is top left corner of object)
// X can be negative if a left column is all gone and it rolls off the screen
void image_render_alienArmy(int32_t x, uint32_t y, struct Alien alienArmy[ALIENS_NUM_ALIENS_WIDE][ALIENS_NUM_ALIENS_HIGH])
{
    for (uint32_t i = 0; i < ALIENS_NUM_ALIENS_WIDE; ++i)
    {
        for (uint32_t j = 0; j < ALIENS_NUM_ALIENS_HIGH; ++j)
        {
            // Assign the position of the aliens
            alienArmy[i][j].x =
                    x + i * (IMAGE_RENDER_ALIEN_ARMY_BLOCK_WIDTH);
            alienArmy[i][j].y =
                    y + j * (IMAGE_RENDER_ALIEN_ARMY_BLOCK_HEIGHT);

            // Don't write the aliens that are gone
            // They could be off the screen and cause it to freeze
            if(!alienArmy[i][j].isWrittenGone)
            {
                image_render_singleAlien(alienArmy[i][j].x, alienArmy[i][j].y,
                                         alienArmy[i][j].alien[alienArmy[i][j].stance]);
            }
            // If alien exploded erase it
            if (alienArmy[i][j].stance == alien_explode)
            {
                alienArmy[i][j].stance = alien_gone;
            }
            else if (alienArmy[i][j].stance == alien_gone) // If the stance is gone, it has been erased and doesn't need to be written any more
            {
                alienArmy[i][j].isWrittenGone = true;
            }

        }
    }
}

// Render an alien bullet at a certain x, y coordinate (x, y is top left corner of object)
void image_render_alienBullet(uint32_t x, uint32_t y, const uint32_t *bulletType)
{
    drawObject(x, y, IMAGE_RENDER_ALIEN_BULLET_HEIGHT, IMAGE_RENDER_ALIEN_BULLET_WIDTH, IMAGE_RENDER_ALIEN_MAGNIFY,
               IMAGE_RENDER_ALIEN_BULLET_COLOR, bulletType);
}

// Render the tank at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tank(uint32_t x, uint32_t y, const uint32_t* tankState)
{
    drawObject(x, y, IMAGE_RENDER_TANK_HEIGHT, IMAGE_RENDER_TANK_WIDTH, IMAGE_RENDER_TANK_MAGNIFY, IMAGE_RENDER_TANK_COLOR,
               tankState);
}

// Render the tank bullet at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tankBullet(uint32_t x, uint32_t y)
{
    drawObject(x, y, IMAGE_RENDER_ALIEN_BULLET_HEIGHT, IMAGE_RENDER_ALIEN_BULLET_WIDTH,
               TANK_BULLET_MAGNIFY, IMAGE_RENDER_TANK_BULLET_COLOR, tankbullet_1x7);
}

// Render the tank bullet erased at a certain x, y coordinate (x, y is top left corner of object)
void image_render_tankBulletGone(uint32_t x, uint32_t y)
{
    drawObject(x, y, IMAGE_RENDER_ALIEN_BULLET_HEIGHT, IMAGE_RENDER_ALIEN_BULLET_WIDTH,
               TANK_BULLET_MAGNIFY, IMAGE_RENDER_TANK_BULLET_COLOR, tankbullet_gone_1x7);
}

// Render a bunker at a certain x, y coordinate (x, y is top left corner of object)
void image_render_bunker(uint32_t x, uint32_t y)
{
    drawObject(x, y, IMAGE_RENDER_BUNKER_HEIGHT, IMAGE_RENDER_BUNKER_WIDTH, IMAGE_RENDER_BUNKER_MAGNIFY, IMAGE_RENDER_BUNKER_COLOR,
               bunker_24x18);
}

// Render a bunker portion at a certain x, y coordinate (x, y is top left corner of object)
void image_render_bunkerPortion(uint32_t x, uint32_t y, const uint32_t *bunkerPortion)
{
    drawObject(x, y, IMAGE_RENDER_BUNKER_PORTION_HEIGHT, IMAGE_RENDER_BUNKER_PORTION_WIDTH,
               IMAGE_RENDER_BUNKER_MAGNIFY, IMAGE_RENDER_BUNKER_COLOR, bunkerPortion);
}

// Render the score and lives
void image_render_topBar()
{
    displayString(SCORE_START_X, SCORE_START_Y, SPRITE_WHITE,
                  IMAGE_RENDER_SPRITE_MAGNIFY, SCORE_STR);
    image_render_lives(globals_getNumLives());
    image_render_updateScore();
}

// Render the score at a certain x, y coordinate (x, y is top left corner of object)
// Used for top bar and high scores
void image_render_score(uint32_t x, uint32_t y, uint32_t score)
{
    for (int32_t i = 4; i >= 0; --i)
    {
        uint32_t digitValue = score % DECIMAL;
        drawSpriteGreen(x + SPRITE_LETTER_WIDTH * i, y, getNumberSprite(digitValue));
        score /= DECIMAL;
    }
}

// Redraw the score
void image_render_updateScore()
{
    image_render_score(SCORE_TEN_THOUSANDS_X, SCORE_START_Y, globals_getScore());
}

// Redraw the lives
void image_render_lives(uint8_t num_lives)
{
    displayString(LIVES_X, LIVES_Y, SPRITE_WHITE,
                  IMAGE_RENDER_SPRITE_MAGNIFY, LIVES_STR);

    for (uint8_t i = 0; i < MAX_NUM_LIVES; ++i)
    {
        if (num_lives > i)
            image_render_tank(LIFE_MARKER_X + i * LIFE_MARKER_X_WIDTH, LIFE_MARKER_Y, tank_17x8);
        else
            image_render_tank(LIFE_MARKER_X + i * LIFE_MARKER_X_WIDTH, LIFE_MARKER_Y, tank_gone_17x8);
    }
}

// Render the game over screen
void image_render_gameOver()
{
    displayString(GAME_OVER_START_X, GAME_OVER_START_Y, SPRITE_WHITE,
                  GAME_OVER_MAGNIFY, GAME_OVER_STR);
}

// Render the phrase enter your name
void image_render_enterName(uint32_t rgbValue, const char *initials)
{
    displayString(NAME_X, NAME_Y, rgbValue,
                  IMAGE_RENDER_SPRITE_MAGNIFY, ENTER_NAME_STR);
    displayString(INITIALS_X, INITIALS_Y, rgbValue,
                  IMAGE_RENDER_SPRITE_MAGNIFY, initials);

}

// Display the high score at a x, y coordinate (top left)
void display_highScores(uint32_t x, uint32_t y, const struct HighScore *highScores)
{
    uint32_t scores_x = x + HIGH_SCORES_SCORES_X_OFFSET;
    for (int i = 0; i < CONTROL_NUM_HIGH_SCORES; ++i)
    {
        uint32_t row_y = y + i * ROW_OFFSET;

        // Display initials
        displayString(x, row_y, SPRITE_WHITE, IMAGE_RENDER_SPRITE_MAGNIFY, highScores[i].initials);

        // Display Score
        image_render_score(scores_x, row_y, highScores[i].score);
    }
}

// Render the high scores
void image_render_highScores(const struct HighScore *highScores)
{
    // Erase enter name
    image_render_enterName(IMAGE_RENDER_BACKGROUND_COLOR, ERASE_INITIALS);

    // Display high scores string
    displayString(HIGH_SCORES_STR_X, HIGH_SCORES_STR_Y, SPRITE_WHITE,
                  IMAGE_RENDER_SPRITE_MAGNIFY, HIGH_SCORES_STR);
    // Display scores
    display_highScores(HIGH_SCORES_INITIALS_START_X, HIGH_SCORES_INITIALS_START_Y, highScores);
}
