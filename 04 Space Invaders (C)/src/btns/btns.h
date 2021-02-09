#ifndef BTNS_H
#define BTNS_H

#include <stdint.h>


/*********************************** macros ***********************************/
#define UIO_EXAMPLE_ERROR       -1      //error return value
#define UIO_EXAMPLE_SUCCESS     0       //success return value


/**************************** function prototypes *****************************/
// initializes the uio driver
int32_t btns_init(char devDevice[]);

// write to a register of the UIO device
void btns_write(uint32_t offset, uint32_t value);

// read which buttons were pressed
uint32_t btns_read();

// acknowledge a button interrupt occurred
void btns_ack_interrupt();

// close the UIO device
void btns_exit();

#endif // BTNS_H