#include "intc.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#define UIO_MMAP_SIZE 0x1000
#define ENABLE_IRQ 0x00000001   // enables the irq linux uio

#define IPR_OFFSET 0X04         // IPR is the ISR & IER, used to tell which interrupt occurred
#define IER_OFFSET 0X08         // Offset for enabling interrupts
#define IAR_OFFSET 0x0C         // Offset for acknowledging interrupts
#define SIE_OFFSET 0x10         // Offset to access SIE register
#define CIE_OFFSET 0x14         // Offset to access CIE register
#define MER_OFFSET 0x1C         // Offset to access MER register

#define ENABLE_ALL_INTC_MASK 0b1110 // Value to enable all interrupts (using 4)
#define MER_ENABLE_VALUE 0b11      // Value written to the MER register to allow interrupts
#define DISABLE_VALUE 0b0

/*********************************globals**********************************/
static int f;
static char *ptr;


/********************************functions*********************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t intc_init(char devDevice[])
{
    // open the device
    f = open(devDevice, O_RDWR);
    if (f <= INTC_SUCCESS)
        return INTC_ERROR;

    // memory map the physical address of the hardware into virtual address space
    ptr = mmap(0, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,f,0);
    if (ptr == MAP_FAILED)
        return INTC_ERROR;

    /* put hardware setup here */
    intc_enable_uio_interrupts();
    intc_irq_enable(ENABLE_ALL_INTC_MASK);

    return INTC_SUCCESS;
}

// Called to exit the driver (unmap and close UIO file)
void intc_exit()
{
    munmap(ptr, UIO_MMAP_SIZE);
    close(f);
}

// write to a register of the UIO device
void intc_write(uint32_t offset, uint32_t value)
{
    //the address is cast as a pointer so it can be dereferenced
    *((volatile uint32_t *) (ptr + offset)) = value;
}

// read from a register of the UIO device
uint32_t intc_read(uint32_t offset)
{
    return *((volatile uint32_t *) (ptr + offset));
}


// This function will block until an interrupt occurs
// Returns: Bitmask of activated interrupts
uint32_t intc_wait_for_interrupt()
{
    uint32_t pending_int;
    read(f, &pending_int, sizeof(pending_int)); // Waits for an interrupts to happen

    pending_int = intc_read(IPR_OFFSET);        // Tells you which interrupt occurred
    return pending_int;
}

// Acknowledge interrupt(s) in the interrupt controller
// irq_mask: Bitmask of interrupt lines to acknowledge.
void intc_ack_interrupt(uint32_t irq_mask)
{
    intc_write(IAR_OFFSET, irq_mask);
    uint32_t buffer = ENABLE_IRQ;                   // Acknowledge to the controller that an interrupt occurred
    ssize_t w = write(f, &buffer, sizeof(buffer));  // Acknowledge to the UIO device that an interrupt occurred
}

// Instruct the UIO to enable interrupts for this device in Linux
// (see the UIO documentation for how to do this)
void intc_enable_uio_interrupts()
{
    uint32_t buffer = ENABLE_IRQ;
    ssize_t w = write(f, &buffer, sizeof(buffer));
}

// Enable interrupt line(s)
// irq_mask: Bitmask of lines to enable
// This function only enables interrupt lines, ie, a 0 bit in irq_mask
//	will not disable the interrupt line
void intc_irq_enable(uint32_t irq_mask)
{
    intc_write(IER_OFFSET, irq_mask);
    intc_write(MER_OFFSET, MER_ENABLE_VALUE);
}

// Same as intc_irq_enable, except this disables interrupt lines
void intc_irq_disable(uint32_t irq_mask)
{
    intc_write(IER_OFFSET, DISABLE_VALUE);
    intc_write(MER_OFFSET, DISABLE_VALUE);
}

