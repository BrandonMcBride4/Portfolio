#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mitch Adams and Brandon McBride");
MODULE_DESCRIPTION("ECEn 427 Audio Driver");

#define MODULE_NAME "audio-codec-device" // module name
#define DEVICE_NAME "BM_audio_device"
#define MEM_NAME "audio_mem_addr"   // Name for reserving memory
#define COMPATIBLE_STRING "byu,ecen427-audio_codec" // Compatible string
#define PHYSICAL_ADDR 0x43C20000 // Physical address of audio-codec
#define SIZE_OF_HARDWARE_ADDRESS_RANGE 0x10000 // SIZE_OF_HARDWARE_ADDRESS_RANGE
#define NUM_CHAR_DEVICES 1 // We are setting up one char driver
#define STATUS_REGISTER_OFFSET 0x10 // Interrupt offset from audio hardware
#define DATA_TX_R_OFFSET 0x08
#define DATA_TX_L_OFFSET 0x0C
#define DISABLE_INTERRUPT 0 // Value to disable interrupts
#define ENABLE_INTERRUPT 1  // Value to enable interrupts
#define MINOR_NUM 0 // Minor number
#define SUCCESS 0  // Returns this if init was successful
#define ERROR -1   // Returns this if init wasn't successful

#define STATUS_LEFT_MASK 0x7FE
#define STATUS_LEFT_SHIFT 1
#define STATUS_RIGHT_MASK 0x1FF800
#define STATUS_RIGHT_SHIFT 11
#define FIFO_MAX_ENTRY 1024
#define NUM_BYTES_AUDIO_BUFFER 4

#define TRUE 1
#define FALSE 0
#define LOOPING_MAGIC 'l'
#define LOOPING_NUM 1
#define LOOPING_CMD _IO(LOOPING_MAGIC, LOOPING_NUM)
#define NO_LOOPING_NUM 0
#define NO_LOOPING_CMD _IO(LOOPING_MAGIC, NO_LOOPING_NUM)

// Function declarations
static int audio_init(void); // This is called when Linux loads your driver
static void audio_exit(void); // This is called when Linux unloads your driver
static ssize_t audio_read(struct file *, char __user *, size_t, loff_t *); // Overwriting the user read function
static ssize_t audio_write(struct file *, const char __user *, size_t, loff_t *); // Overwriting the user write function
static int audio_probe(struct platform_device *pdev); // Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_remove(struct platform_device *pdev); // Undoes what probe sets up
static irqreturn_t audio_isr(int, void *); // The handler for an interrupt


module_init(audio_init); // Initializes driver
module_exit(audio_exit); // Undoes what init does

struct audio_device
{
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device *pdev;       // Platform device pointer
    struct device *dev;                 // device (/dev)

    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size 
    u32 *virt_addr;                     // Virtual address

    // Add any items to this that you need
};

static struct class * audio_class; // Class pointer for audio driver
static struct audio_device my_audio_struct; // The struct containing info for audio

static dev_t assignedDeviceNumber; // The major and minor number of device

static struct file_operations fops; // Used to overwrite read and write

static struct resource *mem_res; // Resource struct to reserve memory
static struct resource *irq_res; // Resource struct to set up IRQ

static s32* audioBuffer; // Buffer created in audio write that stores
static u32 audioBufferIndex;
static size_t audioBufferSize;

static int isLoopingEnabled = FALSE;

// write to a register of the device
void audio_write_register(uint32_t offset, uint32_t value)
{
    //the address is cast as a pointer so it can be dereferenced
    iowrite32(value ,(void *)((char*)(my_audio_struct.virt_addr) + offset));
}

// read from a register of the device
u32 audio_read_register(uint32_t offset)
{
    return ioread32((void *)((char*)(my_audio_struct.virt_addr) + offset));
}

// User read function
static ssize_t audio_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    pr_info("%s: Entered audio_read function\n", MODULE_NAME);
    return audioBuffer != NULL;
}

// User write function
static ssize_t audio_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    pr_info("%s: Entered audio_write function\n", MODULE_NAME);

//    Immediately disable interrupts from the audio core.
    audio_write_register(STATUS_REGISTER_OFFSET, DISABLE_INTERRUPT);

//    Free the buffer used to store the old sound sample (if applicable) (kfree) and allocate a buffer for the new clip (kmalloc).
    if (audioBuffer != NULL)
    {
        pr_info("%s: Freeing audio buffer\n", MODULE_NAME);
        kfree(audioBuffer);
        audioBuffer = NULL; // Freeing it doesn't set it to null
    }
    audioBufferIndex = 0; // Reset the index
    audioBuffer = kmalloc(count, GFP_KERNEL);
    audioBufferSize = count;

//    Copy the audio data from userspace to your newly allocated buffer (including safety checks on the userspace pointer) - LDD page 64.
    unsigned long bytes_left_to_write = copy_from_user(audioBuffer, buf, count);
    pr_info("%s: bytes_left_to_write: %ld", MODULE_NAME, bytes_left_to_write);

//    Make sure the audio core has interrupts enabled.
    audio_write_register(STATUS_REGISTER_OFFSET, ENABLE_INTERRUPT);
    return (count - bytes_left_to_write);
}

// Called when an audio interrupt occurs
static irqreturn_t audio_isr(int irq, void *dev_id)
{
    pr_info("%s: Entered audio_isr\n", MODULE_NAME);

//    Determine how much free space is in the audio FIFOs and fill them up with the next audio samples to be played.
    audio_write_register(STATUS_REGISTER_OFFSET, DISABLE_INTERRUPT); // Disable interrupts while getting status
    u32 status = audio_read_register(STATUS_REGISTER_OFFSET);
    u32 left = (status & STATUS_LEFT_MASK) >> STATUS_LEFT_SHIFT; // Sets all bits to 0 except bits 20-11 and shifts them
    u32 right = (status & STATUS_RIGHT_MASK) >> STATUS_RIGHT_SHIFT; // Sets all bits to 0 except 10-1 and shifts them
    audio_write_register(STATUS_REGISTER_OFFSET, ENABLE_INTERRUPT); // Re-enable interrupts

//    pr_info("%s: Num of values in left FIFO: %d\n", MODULE_NAME, left);
//    pr_info("%s: Num of values in right FIFO: %d\n", MODULE_NAME, right);

    if (left != right)
    {
        pr_err("ERROR: LEFT AND RIGHT FIFOS ARE DIFFERENT COUNT: left: %d, right: %d\n", left, right);
    }

//    while(1024-left > 2)
    for (int i = 0; i < FIFO_MAX_ENTRY - left; ++i)
    {
        //    Once the end of the audio clip is reached, disable interrupts on the audio core.
        if (audioBufferIndex*NUM_BYTES_AUDIO_BUFFER >= audioBufferSize)
        {
            if (!isLoopingEnabled)
            {
                pr_info("%s: Disabling interrupts in ISR, index: %d, bufferSize: %d\n", MODULE_NAME, audioBufferIndex, audioBufferSize);
                audio_write_register(STATUS_REGISTER_OFFSET, DISABLE_INTERRUPT);

                kfree(audioBuffer);
                audioBuffer = NULL; // Freeing it doesn't set it to null
                // End of audio clip, free audio buffer
                pr_info("%s: Freeing audio buffer\n", MODULE_NAME);
            }
            else
            {
                pr_info("%s: Currently looping\n", MODULE_NAME);

            }
            audioBufferIndex = 0;
            pr_info("%s: Returning from ISR\n", MODULE_NAME);
            return 0;
        }
        audio_write_register(STATUS_REGISTER_OFFSET, DISABLE_INTERRUPT); // Disable interrupts while writing to register
        audio_write_register(DATA_TX_L_OFFSET, audioBuffer[audioBufferIndex]);
        audio_write_register(DATA_TX_R_OFFSET, audioBuffer[audioBufferIndex]);
        audioBufferIndex += 1;
        audio_write_register(STATUS_REGISTER_OFFSET, ENABLE_INTERRUPT); // Re-enable interrupts

//        u32 status = audio_read_register(STATUS_REGISTER_OFFSET);
//        u32 left = (status & 0x7FE) >> 1; // Sets all bits to 0 except bits 20-11 and shifts them
//        u32 right = (status & 0x1FF800) >> 11; // Sets all bits to 0 except 10-1 and shifts them

//        pr_info("%s: Num of values in left FIFO: %d\n", MODULE_NAME, left);
//        pr_info("%s: Num of values in right FIFO: %d\n", MODULE_NAME, right);

//        if (left != right)
//        {
//            pr_err("ERROR: LEFT AND RIGHT FIFOS ARE DIFFERENT IN LOOP COUNT: left: %d, right: %d\n", left, right);
//
//        }
    }

    return 0;
}

long audio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case LOOPING_CMD:
            isLoopingEnabled = TRUE;
            pr_info("%s: Looping Enabled\n", MODULE_NAME);
            break;
        case NO_LOOPING_CMD:
            isLoopingEnabled = FALSE;
            pr_info("%s: Looping Disabled\n", MODULE_NAME);
            break;
        default:
            pr_info("%s: Commands not recognized in ioctl\n", MODULE_NAME);
            break;
    }
    return 0;
}

// Struct used to pair the audio platform struct
static const struct of_device_id of_audio_platform_device_match[] = {
        { .compatible = COMPATIBLE_STRING, },
        {},
};

// Overwrites probe and remove
static struct platform_driver audio_platform_driver =
        {
                .probe = audio_probe,
                .remove = audio_remove,
                .driver =
                        {
                                .name = COMPATIBLE_STRING,
                                .of_match_table = of_audio_platform_device_match,
                                .owner = THIS_MODULE,
                        },
        };

// This is called when Linux loads your driver
static int audio_init(void)
{
    pr_info("\n\n%s: Initializing Audio Driver!\n", MODULE_NAME);
    fops.owner = THIS_MODULE;
    fops.read = audio_read;
    fops.write = audio_write;
    fops.unlocked_ioctl = audio_ioctl;

    // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
    pr_info("%s: Allocating chrdev region\n", MODULE_NAME);
    if (alloc_chrdev_region(&assignedDeviceNumber, MINOR_NUM, NUM_CHAR_DEVICES, MODULE_NAME) < 0)
    {
        pr_err("%s: ERROR in allocating chrdev region\n", MODULE_NAME);
        goto errAllocRegion;
    }

    my_audio_struct.minor_num = MINOR(assignedDeviceNumber);


    // Create a device class. -- class_create()
    pr_info("%s: Creating class\n", MODULE_NAME);
    audio_class = class_create(THIS_MODULE, MODULE_NAME);
    if (audio_class == NULL)
    {
        pr_err("%s: ERROR in creating class\n", MODULE_NAME);
        goto errClassCreate;
    }


    // Register the driver as a platform driver -- platform_driver_register
    pr_info("%s: Registering platform driver register\n", MODULE_NAME);
    if (platform_driver_register(&audio_platform_driver))
    {
        pr_err("%s: ERROR in registering platform driver\n", MODULE_NAME);
        goto errPlatformRegister;
    }

    // If any of the above functions fail, return an appropriate linux error code, and make sure
    // you reverse any function calls that were successful.
    pr_info("%s: INIT WAS SUCCESSFUL\n", MODULE_NAME);
    return SUCCESS; //(Success)

    errPlatformRegister:
        pr_info("%s: Destroying class\n", MODULE_NAME);
        class_destroy(audio_class);
    errClassCreate:
        pr_info("%s: unregistering region\n", MODULE_NAME);
        unregister_chrdev_region(assignedDeviceNumber, NUM_CHAR_DEVICES);
    errAllocRegion:
        return ERROR;
}

// This is called when Linux unloads your driver
static void audio_exit(void)
{
    pr_info("%s: Entering audio_exit\n", MODULE_NAME);

    // platform_driver_unregister
    pr_info("%s: unregister platform driver\n", MODULE_NAME);
    platform_driver_unregister(&audio_platform_driver);

    // class_unregister and class_destroy
    pr_info("%s: destory class\n", MODULE_NAME);
    class_destroy(audio_class);

    // unregister_chrdev_region
    pr_info("%s: unregister chrdev region\n", MODULE_NAME);
    unregister_chrdev_region(assignedDeviceNumber, NUM_CHAR_DEVICES);
    pr_info("%s: EXIT SUCCESS\n", MODULE_NAME);
    return;
}

// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev)
{
    pr_info("%s: Entering audio_probe\n", MODULE_NAME);
    my_audio_struct.pdev = pdev;

    // Initialize the character device structure (cdev_init)
    cdev_init(&(my_audio_struct.cdev), &fops);
    my_audio_struct.cdev.owner = THIS_MODULE;

    // Register the character device with Linux (cdev_add)
    pr_info("%s: cdev_add\n", MODULE_NAME);
    if (cdev_add(&(my_audio_struct.cdev), assignedDeviceNumber, NUM_CHAR_DEVICES) < 0)
    {
        pr_err("%s: ERROR in adding cdev\n", MODULE_NAME);
        goto cdevAddError;
    }

    // Create a device file in /dev so that the character device can be accessed from user space (device_create).
    pr_info("%s: device_create\n", MODULE_NAME);
    my_audio_struct.dev = device_create(audio_class, NULL, assignedDeviceNumber, NULL, DEVICE_NAME);
    if (IS_ERR(my_audio_struct.dev))
    {
        pr_err("%s: ERROR in device_create()\n", MODULE_NAME);
        goto deviceCreateError;
    }

    // Get the physical device address from the device tree -- platform_get_resource
    pr_info("%s: get memory resource\n", MODULE_NAME);
    mem_res = platform_get_resource(pdev, IORESOURCE_MEM, NUM_CHAR_DEVICES-1);
    if (mem_res == NULL)
    {
        pr_err("%s: ERROR in platform_get_resource mem\n", MODULE_NAME);
        goto memGetResourceError;
    }

    my_audio_struct.phys_addr = mem_res->start;
    my_audio_struct.mem_size = mem_res->end - mem_res->start+1;


    // Reserve the memory region -- request_mem_region
    pr_info("%s: request mem region\n", MODULE_NAME);
    struct resource *mem_req = request_mem_region(my_audio_struct.phys_addr, my_audio_struct.mem_size, MEM_NAME);
    if (mem_req == NULL)
    {
        pr_err("%s: ERROR in request_mem_region\n", MODULE_NAME);
        goto requestMemError;
    }

    // Get a (virtual memory) pointer to the device -- ioremap
    my_audio_struct.virt_addr = ioremap(mem_res->start, my_audio_struct.mem_size);

    // Get the IRQ number from the device tree -- platform_get_resource
    pr_info("%s: get irq resource\n", MODULE_NAME);
    irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, NUM_CHAR_DEVICES-1);
    if (irq_res == NULL)
    {
        pr_err("%s: ERROR in platform_get_resource irq\n", MODULE_NAME);
        goto irqGetResourceError;
    }

    // Register your interrupt service routine -- request_irq
    pr_info("%s: request irq\n", MODULE_NAME);
    if (request_irq(irq_res->start,
                     audio_isr,
                     irq_res->flags,
                     DEVICE_NAME,
                     &my_audio_struct))
    {
        pr_err("%s: ERROR in request_irq\n", MODULE_NAME);
        goto irqGetResourceError;
    }


//    pr_info("%s: reading interrupt before enable register: %d\n", MODULE_NAME, audio_read_register(STATUS_REGISTER_OFFSET));
//    pr_info("%s: enabling interrupts\n", MODULE_NAME);
    audio_write_register(STATUS_REGISTER_OFFSET, DISABLE_INTERRUPT);

    // If any of the above functions fail, return an appropriate linux error code, and make sure
    // you reverse any function calls that were successful.

    pr_info("%s: PROBE WAS SUCCESSFUL\n", MODULE_NAME);
    pr_info("%s: audio major number: %d\n", MODULE_NAME, MAJOR(assignedDeviceNumber));
    pr_info("%s: audio minor number: %d\n", MODULE_NAME, my_audio_struct.minor_num);
    pr_info("%s: audio physical address: %x\n", MODULE_NAME, my_audio_struct.phys_addr);
    pr_info("%s: audio virtual address: %p\n", MODULE_NAME, my_audio_struct.virt_addr);
    pr_info("%s: audio irq number: %d\n", MODULE_NAME, irq_res->start);
    return SUCCESS;

    irqGetResourceError:
        pr_info("%s: unmapping memory\n", MODULE_NAME);
        iounmap(my_audio_struct.virt_addr);
    requestMemError:
    memGetResourceError:
        pr_info("%s: destroying device\n", MODULE_NAME);
        device_destroy(audio_class, assignedDeviceNumber);
    deviceCreateError:
        pr_info("%s: deleting cdev\n", MODULE_NAME);
        cdev_del(&(my_audio_struct.cdev));
    cdevAddError:
        pr_info("%s: returning from audio_probe with ERROR\n", MODULE_NAME);
        return ERROR;
}

static int audio_remove(struct platform_device *pdev)
{
    pr_info("%s: Entering audio_remove\n", MODULE_NAME);
    // free_irq
    pr_info("%s: freeing irq\n", MODULE_NAME);
    free_irq(irq_res->start, &my_audio_struct);

    // iounmap
    pr_info("%s: unmap\n", MODULE_NAME);
    iounmap(my_audio_struct.virt_addr);

    // release_mem_region
    pr_info("%s: release mem region\n", MODULE_NAME);
    release_mem_region(my_audio_struct.phys_addr, my_audio_struct.mem_size);

    // device_destroy
    pr_info("%s: device destroy\n", MODULE_NAME);
    device_destroy(audio_class, assignedDeviceNumber);

    // cdev_del
    pr_info("%s: cdev del\n", MODULE_NAME);
    cdev_del(&(my_audio_struct.cdev));

    pr_info("%s: REMOVE WAS SUCCESSFUL\n", MODULE_NAME);
    return SUCCESS;
}