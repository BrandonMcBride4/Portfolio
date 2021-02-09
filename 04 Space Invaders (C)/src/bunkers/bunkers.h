#include <stdint.h>

void process_bunker_collision(uint32_t x, uint32_t y);

void bunkers_init();

enum bunker_portion_damage //enum representing the level of damage of a portion of the bunker
{
   destroyed, //Bunker is totally destroyed
   third_damage, //third level of damage
   second_damage, //second level of damage
   first_damage, //first level of damage
   no_damage //no damage done
};
