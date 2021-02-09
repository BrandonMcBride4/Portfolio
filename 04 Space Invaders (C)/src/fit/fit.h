#ifndef FIT_H
#define FIT_H

#include <stdint.h>


/*********************************** macros ***********************************/
#define UIO_EXAMPLE_ERROR       -1      //error return value
#define UIO_EXAMPLE_SUCCESS     0       //success return value


/**************************** function prototypes *****************************/
// initializes the uio driver
int32_t fit_init(char devDevice[]);

// write to a register of the UIO device
void fit_write(uint32_t offset, uint32_t value);

// read from a register of the UIO device
uint32_t fit_read(uint32_t offset);

// close the UIO device
void fit_exit();

#endif // FIT_H