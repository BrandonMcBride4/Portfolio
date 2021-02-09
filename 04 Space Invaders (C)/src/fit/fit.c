#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "fit.h"

#define UIO_EXAMPLE_MMAP_SIZE   0x1000  //size of memory to allocate


/********************************** globals ***********************************/
static int f;        // this is a file descriptor that describes an open UIO device
static char *ptr;    // this is the virtual address of the UIO device registers


/********************************* functions **********************************/
// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t fit_init(char devDevice[])
{

    //open the device
    f = open(devDevice, O_RDWR);
    if (f <= UIO_EXAMPLE_SUCCESS)
    {
        //file descriptors have to be > 0 to be valid
        return UIO_EXAMPLE_ERROR;
    }

    //memory map the physical address of the hardware into virtual address space
    ptr = mmap(0, UIO_EXAMPLE_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED)
    {
        return UIO_EXAMPLE_ERROR;
    }


    return UIO_EXAMPLE_SUCCESS;
}

// write to a register of the UIO device
void fit_write(uint32_t offset, uint32_t value)
{
    //the address is cast as a pointer so it can be dereferenced
    *((volatile uint32_t *) (ptr + offset)) = value;
}

// read from a register of the UIO device
uint32_t fit_read(uint32_t offset)
{
    return *((volatile uint32_t *) (ptr + offset));
}

// close the UIO device
//	this function must be called after all read/write operations are done
//	to properly unmap the memory and close the file descriptor
void fit_exit()
{
    munmap(ptr, UIO_EXAMPLE_MMAP_SIZE);
    close(f);
}