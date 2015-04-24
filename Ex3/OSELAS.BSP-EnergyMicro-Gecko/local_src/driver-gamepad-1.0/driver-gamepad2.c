/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle insert ga	ing this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

#define DEV_NAME "gamepad"
#define DEV_NR_CNT 1
#define GPIO_EVEN_IRQ_LINE 17
#define GPIO_ODD_IRQ_LINE 18



static int __init gamepad_init(void);
static void __exit gamepad_cleanup(void);
static int gp_open(struct inode*, struct file*);
static int gp_release(struct inode*, struct file*);
static ssize_t gp_read(struct file*, char* __user, size_t, loff_t*);
static ssize_t gp_write(struct file* filp, char* __user buff, size_t count, loff_t* offp);
static irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);
static int gp_fasync(int, struct file*, int);



static dev_t dev; //device number
struct cdev gp_cdev;
struct fasync_struct *fasync;
struct class *gp_class;


//static irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);

module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");


static struct file_operations gp_fops = {
	.owner = THIS_MODULE,
	.open = gp_open,
	.release = gp_release,
	.read = gp_read,
	.write = gp_write,
	.fasync = gp_fasync,
};

irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("Handling GPIO interrupt..\n");
	iowrite32(ioread32(irq_remap + GPIO_IF - GPIO_EXTIPSELL), irq_remap + GPIO_IFC - GPIO_EXTIPSELL);

	if(fasync){
		kill_fasync(&fasync, SIGIO, POLL_IN);
	}

	//clear interrupt flag
	return IRQ_HANDLED;
}

static int gp_fasync(int fd, struct file *filp, int mode)
{
	return fasync_helper(fd, filp, mode, &fasync);
}

//INIT gamepad
static int __init gamepad_init(void)
{
	printk("Hello World, here is your module: %c fucking v18\n", DEV_NAME);

	int res = alloc_chrdev_region(&dev, 0, DEV_NR_CNT, DEV_NAME);
	printk("dev: %i\n",dev);

	//major = MAJOR(*dev);
	if (res < 0){
		printk("Failed to register dev\n");
		return -1;
	}
	



	if (request_mem_region((GPIO_PC_BASE + GPIO_PC_MODEL), 1, DEV_NAME) == NULL){
		printk(KERN_ALERT "Error requesting GPIO_PC_MODEL memory region, already in use?\n");
        return -1;
	}
	if (request_mem_region((GPIO_PC_BASE + GPIO_PC_DOUT), 1, DEV_NAME) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DOUT memory region, already in use?\n");
        return -1;
    }
    if (request_mem_region((GPIO_PC_BASE + GPIO_PC_DIN), 1, DEV_NAME) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DIN memory region, already in use?\n");
        return -1;
    }

    iowrite32(0x33333333, (GPIO_PC_BASE + GPIO_PC_MODEL));
	iowrite32(0xFF, (GPIO_PC_BASE + GPIO_PC_DOUT));
	iowrite(0x22222222, (GPIO_PC_BASE + GPIO_EXTIPSELL));
  	



	cdev_init(&gp_cdev, &gp_fops);
	gp_cdev.owner = THIS_MODULE;
	cdev_add(&gp_cdev, dev, DEV_NR_CNT);
	gp_class = class_create(THIS_MODULE, DEV_NAME);
	device_create(gp_class, NULL, dev, NULL, DEV_NAME);

	iowrite32(0xFF, (GPIO_PA_BASE + GPIO_EXTIFALL));
	iowrite32(0x00FF, (GPIO_PA_BASE + GPIO_IEN));
	iowrite32(0xFF, (GPIO_PA_BASE + GPIO_IFC));

	return 0;
}

static void __exit gamepad_cleanup(void)
{
	iowrite32(0x0000, (GPIO_PA_BASE + GPIO_IEN));

	free_irq(GPIO_EVEN_IRQ_LINE, &gp_cdev);
	free_irq(GPIO_ODD_IRQ_LINE, &gp_cdev);

	release_mem_region((GPIO_PC_BASE + GPIO_PC_MODEL), 1);
	release_mem_region((GPIO_PC_BASE + GPIO_PC_DIN), 1);
	release_mem_region((GPIO_PC_BASE + GPIO_PC_DOUT), 1);

	device_destroy(gp_class, dev);
	class_destroy(gp_class);
	cdev_del(&gp_cdev);

	unregister_chrdev_region(dev, DEV_NR_CNT);
	
	printk("Short life for a small module...\n");
}

//Open gamepad
static int gp_open(struct inode *inode, struct file *file)
{
	printk("Opening Gamepad driver..\n\n");	
	return 0;
}

//Close gamepad		
static int gp_release(struct inode* inode, struct file* file)
{
	printk("Closing Gamepad driver..\n\n");
	return 0;
}

static ssize_t gp_read(struct file* file, char* __user buff, size_t count, loff_t* loff_t)
{
	uint32_t data = ioread32((GPIO_PC_BASE + GPIO_PC_DIN));
	copy_to_user(buff, &data, 1);
	return 1;
}

static ssize_t gp_write(struct file* filp, char* __user buff,
        size_t count, loff_t* offp)
{
    printk(KERN_INFO "Writing to buttons doesn't make sense.");
    return 1;
}
