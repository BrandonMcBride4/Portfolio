#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "switches.h"

#define UIO_MMAP_SIZE   0x1000  //size of memory to allocate

#define GPIO_DATA_OFFSET 0x000 // Outputs get written to this register
#define GPIO_TRI_OFFSET  0x004 // Offset to TRI register which indicates its state
#define IP_IER_OFFSET    0x128 // Offset to IP IER register which enables interrupts
#define GIER_OFFSET      0x11C // Offset to Global IER which also needs to be enabled
#define IP_ISR_OFFSET    0x120 // Offset to IP interrupts state register, used to acknowledge interrupts

#define GPIO_TRI_INPUT 1       // Needs to be a 1 to consider the buttons an input
#define IP_IER_ENABLE  1       // Needs to be a 1 to enable interrupts on GPIO 1
#define GIER_ENABLE 0x80000000 // 31st bit needs to be 1 to enable interupts
#define IP_ISR_ENABLE  1       // Needs to be 1 to acknowledge an interrupts on GPIO 1

/********************************** globals ***********************************/
static int f;        // this is a file descriptor that describes an open UIO device
static char *ptr;    // this is the virtual address of the UIO device registers


/********************************* functions **********************************/
// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t switches_init(char devDevice[])
{

    //open the device
    f = open(devDevice, O_RDWR);
    if (f <= UIO_EXAMPLE_SUCCESS)
    {
        //file descriptors have to be > 0 to be valid
        return UIO_EXAMPLE_ERROR;
    }

    //memory map the physical address of the hardware into virtual address space
    ptr = mmap(0, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED)
    {
        return UIO_EXAMPLE_ERROR;
    }

    /* put hardware setup here */
    switches_write(GPIO_TRI_OFFSET, GPIO_TRI_INPUT); // Says we read from the switches
    switches_write(IP_IER_OFFSET, IP_IER_ENABLE);    // Enables interrupts for the switches
    switches_write(GIER_OFFSET, GIER_ENABLE);        // Enables interrupts for the switches


    return UIO_EXAMPLE_SUCCESS;
}

// write to a register of the UIO device
// used in enabling the buttons
void switches_write(uint32_t offset, uint32_t value)
{
    //the address is cast as a pointer so it can be dereferenced
    *((volatile uint32_t *) (ptr + offset)) = value;
}

// read switches
uint32_t switches_read()
{
    return *((volatile uint32_t *) (ptr + GPIO_DATA_OFFSET));
}

// read from a register of the UIO device
uint32_t switches_read_gpio(uint32_t offset)
{
    return *((volatile uint32_t *) (ptr + offset));
}

// acknowledge a switch interrupt occurred
void switches_ack_interrupt()
{
    switches_write(IP_ISR_OFFSET, IP_ISR_ENABLE);
}

// close the UIO device
//	this function must be called after all read/write operations are done
//	to properly unmap the memory and close the file descriptor
void switches_exit()
{
    munmap(ptr, UIO_MMAP_SIZE);
    close(f);
}
