#include "bunkers.h"
#include "../image_render/image_render.h"
#include "../image_render/sprite.h"
#include <stdio.h>
#include <stdbool.h>

#define NUMBER_OF_BUNKERS 4 //Total number of bunkers
#define PORTION_WIDTH IMAGE_RENDER_BUNKER_PORTION_WIDTH * IMAGE_RENDER_BUNKER_MAGNIFY //Width on one portion of a bunker
#define PORTION_HEIGHT IMAGE_RENDER_BUNKER_PORTION_HEIGHT * IMAGE_RENDER_BUNKER_MAGNIFY //Height on one portion of a bunker
#define PORTION_ONE_DISTANCE IMAGE_RENDER_BUNKER_PORTION_WIDTH * IMAGE_RENDER_BUNKER_MAGNIFY //Distance from leftmost side of bunker to the end of the first bunker portion
#define PORTION_TWO_DISTANCE IMAGE_RENDER_BUNKER_PORTION_WIDTH * IMAGE_RENDER_BUNKER_MAGNIFY * 2 //Distance from leftmost side of bunker to the end of the second bunker portion
#define PORTION_THREE_DISTANCE IMAGE_RENDER_BUNKER_PORTION_WIDTH * IMAGE_RENDER_BUNKER_MAGNIFY * 3 //Distance from leftmost side of bunker to the end of the third bunker portion
#define PORTION_FOUR_DISTANCE IMAGE_RENDER_BUNKER_PORTION_WIDTH * IMAGE_RENDER_BUNKER_MAGNIFY * 4 //Distance from leftmost side of bunker to the end of the bunker
#define BUNKER_WIDTH 4 //Number of portions wide the bunker is
#define BUNKER_HEIGHT 3 //Number of portions high the bunker is
#define FIRST_BUNKER 0 //Leftmost bunker
#define SECOND_BUNKER 1 //Second to leftmost bunker
#define THIRD_BUNKER 2 //Second to rightmost bunker
#define FOURTH_BUNKER 3 //Rightmost bunker
#define FIRST_COLUMN 0 //First column of bunker portions
#define SECOND_COLUMN 1 //Second column of bunker portions
#define THIRD_COLUMN 2 //Third column of bunker portions
#define FOURTH_COLUMN 3 //Fourth column of bunker portions
#define FIRST_ROW 0 //First row of bunker portions
#define SECOND_ROW 1 //Second row of bunker portions
#define THIRD_ROW 2 //Third row of bunker portions

static enum bunker_portion_damage bunkerOnePortions[BUNKER_WIDTH][BUNKER_HEIGHT]; //Array that holds damage values of the first bunker
static enum bunker_portion_damage bunkerTwoPortions[BUNKER_WIDTH][BUNKER_HEIGHT]; //Array that holds damage values of the second bunker
static enum bunker_portion_damage bunkerThreePortions[BUNKER_WIDTH][BUNKER_HEIGHT]; //Array that holds damage values of the third bunker
static enum bunker_portion_damage bunkerFourPortions[BUNKER_WIDTH][BUNKER_HEIGHT]; //Array that holds damage values of the fourth bunker


void bunkers_init() { //Function that initializes the bunker arrays
    for (int i = 0; i < BUNKER_WIDTH; i++)//Iterate through all of the portions of the bunkers
    {
        for (int j = 0; j < BUNKER_HEIGHT; j++)
        {
          //set all of the bunkers to having no damage
            bunkerOnePortions[i][j] = no_damage;
            bunkerTwoPortions[i][j] = no_damage;
            bunkerThreePortions[i][j] = no_damage;
            bunkerFourPortions[i][j] = no_damage;
        }
    }
}

void copyArray(uint32_t* dest_array, uint32_t* source_array) { //Copies one array to another
    for (uint32_t i = 0; i < PORTION_WIDTH; i++) { //Only works for when we are passing arrays that represent portion sprites
      dest_array[i] = source_array[i];
    }
}

uint32_t* get_bunker_render(uint32_t row, uint32_t column, enum bunker_portion_damage damage) { //This will get a sprite to render for the bunker portion
    static uint32_t render_image[IMAGE_RENDER_BUNKER_PORTION_HEIGHT]; //This hold the new image to render
    static uint32_t image_mask[IMAGE_RENDER_BUNKER_PORTION_HEIGHT]; //This holds a mask to modify the sprite
    bool masked = false; //This variable indicates if a sprite needs to be masked or not

    switch (damage) //Figure out the level of damage of the sprite
    {
        case no_damage: //Sprite with no damage
          copyArray(render_image, bunkerDamage3_6x6); //Copy the appropriate sprite from sprite.h
          break;
        case first_damage: //Sprite with first level of damage
          copyArray(render_image, bunkerDamage2_6x6); //Copy the appropriate sprite from sprite.h
          break;
        case second_damage://Sprite with second level of damage
          copyArray(render_image, bunkerDamage1_6x6); //Copy the appropriate sprite from sprite.h
          break;
        case third_damage: //Sprite with third level of damage
          copyArray(render_image, bunkerDamage0_6x6); //Copy the appropriate sprite from sprite.h
          break;
        case destroyed: //Sprite that is totally destroyed
          copyArray(render_image, bunkerDamageGone); //Copy the appropriate sprite from sprite.h
          break;
        default: //you've got a problem if you end up here
          break;
    }
    //These are special cases where the sprite is not a perfect square and must be masked
    if (row == FIRST_ROW && column == FIRST_COLUMN) //If it is the first row and first column
    {
        copyArray(image_mask, bunker_upper_left_gone_6x6); //Copy the upper left sprite to the mask value
        masked = true; // Indicate that the sprite needs to be masked
    }
    else if (row == FIRST_ROW && column == FOURTH_COLUMN) //If it is the first row and fourth column
    {
        copyArray(image_mask, bunker_upper_right_gone_6x6);//Copy the upper right sprite to the mask value
        masked = true; // Indicate that the sprite needs to be masked
    }
    else if (row == SECOND_ROW && column == SECOND_COLUMN) //If it is the second row and second column
    {
        copyArray(image_mask, bunker_lower_right_gone_6x6);//Copy the lower left sprite to the mask value
        masked = true; // Indicate that the sprite needs to be masked
    }
    else if (row == SECOND_ROW && column == THIRD_COLUMN)  //If it is the second row and third column
    {
        copyArray(image_mask, bunker_lower_left_gone_6x6);//Copy the lower right sprite to the mask value
        masked = true; // Indicate that the sprite needs to be masked
    }
    else //you've got a problem
    {
    }
    if (masked) { //If does need to be masked, go through each element of the array and mask it with the mask array
      for (int i = 0; i < IMAGE_RENDER_BUNKER_PORTION_HEIGHT; i++) {
          render_image[i] = render_image[i] & image_mask[i];
      }
    }
    return render_image;
  }

void damage_bunker_portion(uint8_t row, uint8_t column, uint8_t bunker_number, uint32_t leftX) //Function that figures out the portion of the array that needs to be damaged
{
   uint32_t* image_render; //This will hold the sprite
   if (bunker_number == FIRST_BUNKER) //If it is the first bunker
   {
      if (bunkerOnePortions[column][row] != destroyed)
      {
          bunkerOnePortions[column][row]--; //decrement the damage level
          image_render = get_bunker_render(row, column, bunkerOnePortions[column][row]);
          image_render_bunkerPortion(leftX + column * PORTION_WIDTH, IMAGE_RENDER_BUNKER_Y + row * PORTION_HEIGHT, image_render);
      }
   }
   else if (bunker_number == SECOND_BUNKER) //If it is the second bunker
   {
      if (bunkerTwoPortions[column][row] != destroyed)
      {
          bunkerTwoPortions[column][row]--; //decrement the damage level
          image_render = get_bunker_render(row, column, bunkerTwoPortions[column][row]);
          image_render_bunkerPortion(leftX + column * PORTION_WIDTH, IMAGE_RENDER_BUNKER_Y + row * PORTION_HEIGHT, image_render);
      }
   }
   else if (bunker_number == THIRD_BUNKER) //If it is the third bunker
   {
      if (bunkerThreePortions[column][row] != destroyed)
      {
          bunkerThreePortions[column][row]--; //decrement the damage level
          image_render = get_bunker_render(row, column, bunkerThreePortions[column][row]);
          image_render_bunkerPortion(leftX + column * PORTION_WIDTH, IMAGE_RENDER_BUNKER_Y + row * PORTION_HEIGHT, image_render);
      }
   }
   else if (bunker_number == FOURTH_BUNKER) //If it is the fourth bunker
   {
      if (bunkerFourPortions[column][row] != destroyed)
      {
          bunkerFourPortions[column][row]--; //decrement the damage level
          image_render = get_bunker_render(row, column, bunkerFourPortions[column][row]);
          image_render_bunkerPortion(leftX + column * PORTION_WIDTH, IMAGE_RENDER_BUNKER_Y + row * PORTION_HEIGHT, image_render);
      }
   }
}

void locate_bunker_portion(uint32_t x, uint32_t y, uint8_t bunker_number, uint32_t leftX) //Find the portion of the bunker to destroy given x and y coordinates
{
    if (x < leftX + PORTION_ONE_DISTANCE) //If bullet is hit in first column
    {
        if (y < IMAGE_RENDER_BUNKER_Y + PORTION_ONE_DISTANCE) // first column, first row
        {
            damage_bunker_portion(FIRST_ROW, FIRST_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_TWO_DISTANCE) // first column, second row
        {
            damage_bunker_portion(SECOND_ROW, FIRST_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_THREE_DISTANCE) // first column, third row
        {
            damage_bunker_portion(THIRD_ROW, FIRST_COLUMN, bunker_number, leftX);
        }
    }
    else if (x < leftX + PORTION_TWO_DISTANCE) //If bullet is hit in second column
    {
        if (y < IMAGE_RENDER_BUNKER_Y + PORTION_ONE_DISTANCE) // second column, first row
        {
            damage_bunker_portion(FIRST_ROW, SECOND_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_TWO_DISTANCE) // second column, second row
        {
            damage_bunker_portion(SECOND_ROW, SECOND_COLUMN, bunker_number, leftX);
        }
        //There is no bunker in the second column, third row
    }
    else if (x < leftX + PORTION_THREE_DISTANCE) //If bullet is hit in third column
    {
        if (y < IMAGE_RENDER_BUNKER_Y + PORTION_ONE_DISTANCE) // third column, first row
        {
            damage_bunker_portion(FIRST_ROW, THIRD_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_TWO_DISTANCE) // third column, second row
        {
            damage_bunker_portion(SECOND_ROW, THIRD_COLUMN, bunker_number, leftX);
        }
        //There is no bunker in the third column, third row
    }
    else if (x < leftX + PORTION_FOUR_DISTANCE) //If bullet is hit in third column
    {
        if (y < IMAGE_RENDER_BUNKER_Y + PORTION_ONE_DISTANCE) // fourth column, first row
        {
            damage_bunker_portion(FIRST_ROW, FOURTH_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_TWO_DISTANCE) // fourth column, second row
        {
            damage_bunker_portion(SECOND_ROW, FOURTH_COLUMN, bunker_number, leftX);
        }
        else if (y < IMAGE_RENDER_BUNKER_Y + PORTION_THREE_DISTANCE) // fourth column, third row
        {
            damage_bunker_portion(THIRD_ROW, FOURTH_COLUMN, bunker_number, leftX);
        }
    }
}

void process_bunker_collision(uint32_t x, uint32_t y) { //Find the bunker that was shot given x and y coordinates
  if (x < IMAGE_RENDER_BUNKER_1_X) //First bunker was hit
  {
    locate_bunker_portion(x, y, FIRST_BUNKER, IMAGE_RENDER_BUNKER_0_X);
  }
  else if (x < IMAGE_RENDER_BUNKER_2_X)  //Second bunker was hit
  {
    locate_bunker_portion(x, y, SECOND_BUNKER, IMAGE_RENDER_BUNKER_1_X);
  }
  else if (x < IMAGE_RENDER_BUNKER_3_X) //Third bunker was hit
  {
    locate_bunker_portion(x, y, THIRD_BUNKER, IMAGE_RENDER_BUNKER_2_X);
  }
  else  //Fourth bunker was hit
  {
    locate_bunker_portion(x, y, FOURTH_BUNKER, IMAGE_RENDER_BUNKER_3_X);
  }
}
