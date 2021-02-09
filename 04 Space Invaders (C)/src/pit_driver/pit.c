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

#include <linux/device.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mitch Adams and Brandon McBride");
MODULE_DESCRIPTION("ECEn 427 pit Driver");

#define MODULE_NAME "pit-device" // module name
#define DEVICE_NAME "BM_pit_device"
#define MEM_NAME "pit_mem_addr"   // Name for reserving memory
#define COMPATIBLE_STRING "byu,ecen427-pit" // Compatible string
#define PHYSICAL_ADDR 0x43C30000 // Physical address of pit
#define SIZE_OF_HARDWARE_ADDRESS_RANGE 0x10000 // SIZE_OF_HARDWARE_ADDRESS_RANGE
#define NUM_CHAR_DEVICES 1 // We are setting up one char driver
#define MINOR_NUM 0 // Minor number
#define SUCCESS 0  // Returns this if init was successful
#define ERROR -1   // Returns this if init wasn't successful

#define CONTROL_VALUE  0x03 // This enables interrupts and starts the timer
#define CONTROL_OFFSET 0x00 // Register offset for the control
#define PERIOD_OFFSET  0x04 // Register offset for the period

#define PERIOD_ATTR_NAME "period_attribute"
#define TIMER_ENABLE_ATTR_NAME "timer_enable"
#define INTERRUPT_ENABLE_ATTR_NAME "interrupt_enable"
#define ATTR_PERMISSION 0666

#define TRUE 1
#define FALSE 0

// Function declarations
static int pit_init(void); // This is called when Linux loads your driver
static void pit_exit(void); // This is called when Linux unloads your driver
static int pit_probe(struct platform_device *pdev); // Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int pit_remove(struct platform_device *pdev); // Undoes what probe sets up


module_init(pit_init); // Initializes driver
module_exit(pit_exit); // Undoes what init does

struct pit_device
{
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device *pdev;       // Platform device pointer
    struct device *dev;                 // device (/dev)

    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size
    u32 *virt_addr;                     // Virtual address

};

static struct class * pit_class; // Class pointer for pit driver
static struct pit_device my_pit_struct; // The struct containing info for pit

static dev_t assignedDeviceNumber; // The major and minor number of device
static struct resource *mem_res; // Resource struct to reserve memory

static struct device_attribute dev_attr_period;
static struct device_attribute dev_attr_timer_enable;
static struct device_attribute dev_attr_interrupt_enable;


// Struct used to pair the pit platform struct
static const struct of_device_id of_pit_platform_device_match[] = {
        { .compatible = COMPATIBLE_STRING, },
        {},
};

// Overwrites probe and remove
static struct platform_driver pit_platform_driver =
        {
                .probe = pit_probe,
                .remove = pit_remove,
                .driver =
                        {
                                .name = COMPATIBLE_STRING,
                                .of_match_table = of_pit_platform_device_match,
                                .owner = THIS_MODULE,
                        },
        };


// write to a register of the device
void pit_write_register(uint32_t offset, uint32_t value)
{
    //the address is cast as a pointer so it can be dereferenced
    iowrite32(value ,(void *)((char*)(my_pit_struct.virt_addr) + offset));
}

// read from a register of the device
u32 pit_read_register(uint32_t offset)
{
    return ioread32((void *)((char*)(my_pit_struct.virt_addr) + offset));
}

ssize_t pit_read(struct device * dev, struct device_attribute * attr, char * buf)
{
    u32 value;

    if (attr == &dev_attr_period)
    {
        value = pit_read_register(PERIOD_OFFSET);
    }
    else if (attr == &dev_attr_timer_enable)
    {
        value = pit_read_register(CONTROL_OFFSET);
        value = value & 0x01 == 0x01;

    }
    else if (attr == &dev_attr_interrupt_enable)
    {
        value = pit_read_register(CONTROL_OFFSET);
        value = value & 0x02 == 0x02;
    }

    scnprintf(buf, 32, "%d\n", value);
    return 32;
}

ssize_t pit_write(struct device * dev, struct device_attribute * attr, const char * buf, size_t count)
{
    char ** end;
    u32 value = simple_strtol(buf, end, 10);

    if (attr == &dev_attr_period)
    {
        pit_write_register(PERIOD_OFFSET, value);
    }
    else if (attr == &dev_attr_timer_enable)
    {
        u32 reg = pit_read_register(CONTROL_OFFSET);
        reg = value == 0? (reg & (0xFFFFFFFF - 0x01)): (reg | 0x01);
        pit_write_register(CONTROL_OFFSET, reg);
    }
    else if (attr == &dev_attr_interrupt_enable)
    {
        u32 reg = pit_read_register(CONTROL_OFFSET);
        reg = value == 0? (reg & (0xFFFFFFFF - 0x02)): (reg | 0x02);
        pit_write_register(CONTROL_OFFSET, reg);
    }
    return count;
}


// This is called when Linux loads your driver
static int pit_init(void)
{
    pr_info("\n\n%s: Initializing Pit Driver!\n", MODULE_NAME);

    dev_attr_period.attr.name = PERIOD_ATTR_NAME;
    dev_attr_period.attr.mode = ATTR_PERMISSION;
    dev_attr_period.show = pit_read;
    dev_attr_period.store = pit_write;

    dev_attr_timer_enable.attr.name = TIMER_ENABLE_ATTR_NAME;
    dev_attr_timer_enable.attr.mode = ATTR_PERMISSION;
    dev_attr_timer_enable.show = pit_read;
    dev_attr_timer_enable.store = pit_write;

    dev_attr_interrupt_enable.attr.name = INTERRUPT_ENABLE_ATTR_NAME;
    dev_attr_interrupt_enable.attr.mode = ATTR_PERMISSION;
    dev_attr_interrupt_enable.show = pit_read;
    dev_attr_interrupt_enable.store = pit_write;

    // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
    pr_info("%s: Allocating chrdev region\n", MODULE_NAME);
    if (alloc_chrdev_region(&assignedDeviceNumber, MINOR_NUM, NUM_CHAR_DEVICES, MODULE_NAME) < 0)
    {
        pr_err("%s: ERROR in allocating chrdev region\n", MODULE_NAME);
        goto errAllocRegion;
    }

    my_pit_struct.minor_num = MINOR(assignedDeviceNumber);


    // Create a device class. -- class_create()
    pr_info("%s: Creating class\n", MODULE_NAME);
    pit_class = class_create(THIS_MODULE, MODULE_NAME);
    if (pit_class == NULL)
    {
        pr_err("%s: ERROR in creating class\n", MODULE_NAME);
        goto errClassCreate;
    }


    // Register the driver as a platform driver -- platform_driver_register
    pr_info("%s: Registering platform driver register\n", MODULE_NAME);
    if (platform_driver_register(&pit_platform_driver))
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
    class_destroy(pit_class);
    errClassCreate:
    pr_info("%s: unregistering region\n", MODULE_NAME);
    unregister_chrdev_region(assignedDeviceNumber, NUM_CHAR_DEVICES);
    errAllocRegion:
    return ERROR;
}

// This is called when Linux unloads your driver
static void pit_exit(void)
{
    pr_info("%s: Entering pit_exit\n", MODULE_NAME);

    // platform_driver_unregister
    pr_info("%s: unregister platform driver\n", MODULE_NAME);
    platform_driver_unregister(&pit_platform_driver);

    // class_unregister and class_destroy
    pr_info("%s: destory class\n", MODULE_NAME);
    class_destroy(pit_class);

    // unregister_chrdev_region
    pr_info("%s: unregister chrdev region\n", MODULE_NAME);
    unregister_chrdev_region(assignedDeviceNumber, NUM_CHAR_DEVICES);
    pr_info("%s: EXIT SUCCESS\n", MODULE_NAME);
    return;
}

// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int pit_probe(struct platform_device *pdev)
{
    pr_info("%s: Entering pit_probe\n", MODULE_NAME);
    my_pit_struct.pdev = pdev;

    // Initialize the character device structure (cdev_init)
//    cdev_init(&(my_pit_struct.cdev), &fops);
    my_pit_struct.cdev.owner = THIS_MODULE;

    // Register the character device with Linux (cdev_add)
    pr_info("%s: cdev_add\n", MODULE_NAME);
    if (cdev_add(&(my_pit_struct.cdev), assignedDeviceNumber, NUM_CHAR_DEVICES) < 0)
    {
        pr_err("%s: ERROR in adding cdev\n", MODULE_NAME);
        goto cdevAddError;
    }

    // Create a device file in /dev so that the character device can be accessed from user space (device_create).
    pr_info("%s: device_create\n", MODULE_NAME);
    my_pit_struct.dev = device_create(pit_class, NULL, assignedDeviceNumber, NULL, DEVICE_NAME);
    if (IS_ERR(my_pit_struct.dev))
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

    my_pit_struct.phys_addr = mem_res->start;
    my_pit_struct.mem_size = mem_res->end - mem_res->start+1;


    // Reserve the memory region -- request_mem_region
    pr_info("%s: request mem region\n", MODULE_NAME);
    struct resource *mem_req = request_mem_region(my_pit_struct.phys_addr, my_pit_struct.mem_size, MEM_NAME);
    if (mem_req == NULL)
    {
        pr_err("%s: ERROR in request_mem_region\n", MODULE_NAME);
        goto requestMemError;
    }

    // Get a (virtual memory) pointer to the device -- ioremap
    my_pit_struct.virt_addr = ioremap(mem_res->start, my_pit_struct.mem_size);


    // create file
    pr_info("%s: create sysfs files\n", MODULE_NAME);
    if(sysfs_create_file(&my_pit_struct.dev->kobj, &dev_attr_period.attr))
    {
        pr_info("%s: ERROR in creating sysfs file\n", MODULE_NAME);
        goto fileError;
    }

    if(sysfs_create_file(&my_pit_struct.dev->kobj, &dev_attr_timer_enable.attr))
    {
        pr_info("%s: ERROR in creating sysfs file\n", MODULE_NAME);
        goto fileError;
    }

    if(sysfs_create_file(&my_pit_struct.dev->kobj, &dev_attr_interrupt_enable.attr))
    {
        pr_info("%s: ERROR in creating sysfs file\n", MODULE_NAME);
        goto fileError;
    }


//    pit_write_register(CONTROL_OFFSET, CONTROL_VALUE); // Initialize control register


    pr_info("%s: PROBE WAS SUCCESSFUL\n", MODULE_NAME);
    pr_info("%s: pit major number: %d\n", MODULE_NAME, MAJOR(assignedDeviceNumber));
    pr_info("%s: pit minor number: %d\n", MODULE_NAME, my_pit_struct.minor_num);
    pr_info("%s: pit physical address: %x\n", MODULE_NAME, my_pit_struct.phys_addr);
    pr_info("%s: pit virtual address: %p\n", MODULE_NAME, my_pit_struct.virt_addr);
    return SUCCESS;

    fileError:
        pr_info("%s: unmap\n", MODULE_NAME);
        iounmap(my_pit_struct.virt_addr);
    requestMemError:
    memGetResourceError:
        pr_info("%s: destroying device\n", MODULE_NAME);
        device_destroy(pit_class, assignedDeviceNumber);
    deviceCreateError:
        pr_info("%s: deleting cdev\n", MODULE_NAME);
        cdev_del(&(my_pit_struct.cdev));
    cdevAddError:
        pr_info("%s: returning from pit_probe with ERROR\n", MODULE_NAME);
    return ERROR;
}

static int pit_remove(struct platform_device *pdev)
{
    pr_info("%s: Entering pit_remove\n", MODULE_NAME);

    // remove file
    pr_info("%s: remove sysfs files\n", MODULE_NAME);
    sysfs_remove_file(&my_pit_struct.dev->kobj, &dev_attr_period.attr);
    sysfs_remove_file(&my_pit_struct.dev->kobj, &dev_attr_timer_enable.attr);
    sysfs_remove_file(&my_pit_struct.dev->kobj, &dev_attr_interrupt_enable.attr);


    // iounmap
    pr_info("%s: unmap\n", MODULE_NAME);
    iounmap(my_pit_struct.virt_addr);

    // release_mem_region
    pr_info("%s: release mem region\n", MODULE_NAME);
    release_mem_region(my_pit_struct.phys_addr, my_pit_struct.mem_size);

    // device_destroy
    pr_info("%s: device destroy\n", MODULE_NAME);
    device_destroy(pit_class, assignedDeviceNumber);

    // cdev_del
    pr_info("%s: cdev del\n", MODULE_NAME);
    cdev_del(&(my_pit_struct.cdev));

    pr_info("%s: REMOVE WAS SUCCESSFUL\n", MODULE_NAME);
    return SUCCESS;
}
