#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdint.h>


/*********************************** macros ***********************************/
#define UIO_EXAMPLE_ERROR       -1      //error return value
#define UIO_EXAMPLE_SUCCESS     0       //success return value


/**************************** function prototypes *****************************/
// initializes the uio driver
int32_t switches_init(char devDevice[]);

// write to a register of the UIO device
void switches_write(uint32_t offset, uint32_t value);

// read which switches were pressed
uint32_t switches_read();

// acknowledge a switches interrupt occurred
void switches_ack_interrupt();

// close the UIO device
void switches_exit();

#endif // SWITCHES_H