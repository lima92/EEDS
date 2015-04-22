/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>

#include <asm/signal.h>

#include "efm32gg.h"

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

#define DEV_NAME "gamepad"

void setup_GPIO();
void setup_interrupts();

static int gp_open(struct inode*, struct file*);
static int gp_release(struct inode*, struct file*);
static ssize_t gp_read(struct file*, char* __user, size_t, loff_t*);
static ssize_t gp_write(struct file*, char* __user, size_t, loff_t*);

uint32_t *err, *err_odd, *err_even, *handler;
uint32_t *ioremap;
struct class *gp_class;
struct device *gp_device;
dev_t *dev; //device number

struct gamepad_dev{
	struct gamepad_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	unsigned int acces_key;
	struct semaphore sem;
	struct cdev cdev;
} gp_dev;

struct file_operations gp_fops = {
	.owner = THIS_MODULE,
//	.read = gp_read,
//	.write = gp_write,
//	.ioctl = gp_ioctl,
//	.open = gp_open,
//	.release = gp_release,
};




static int __init gamepad_init(void)
{
	printk("Hello World, here is your module: %c fucking v13\n", DEV_NAME);


	int err_reg = alloc_chrdev_region(&dev, 0, 2, DEV_NAME);
	printk("dev: %i\n",dev);
	printk("*deb: %i\n",*dev);

	if(!err_reg){
		printk("CharDev reg successfull\n");
	}else{
		printk("Failed to register dev\n");
		printk("err: %i\n",err_reg);
		return err_reg;
	}
	

	cdev_init(&gp_dev.cdev, &gp_fops);
	gp_dev.cdev.owner = THIS_MODULE;
	printk(KERN_NOTICE "gp_dev.cdev.owner: %i", gp_dev.cdev.owner);
	int err_cdev = cdev_add (&gp_dev.cdev, *dev, 1);
	if(err_cdev){
		printk(KERN_NOTICE "Error %d adding dev%d", err_cdev, 0);
	}else{
		printk(KERN_NOTICE "cdev added successfully!/n");
	}

	err = request_mem_region(GPIO_PC_BASE + GPIO_PC_DIN, 32, DEV_NAME);
	
	if (*err == NULL){
		printk("Failure\n");
		return -1;
	}
	else{
		printk("GPIO INPUT SUCCESS\n");
	}


	//MOVE TO OPEN?
	setup_GPIO();
	setup_interrupts();

	//Create device file
	gp_class = class_create(THIS_MODULE, DEV_NAME);
	gp_device = device_create(gp_class, NULL, dev, NULL, DEV_NAME);
	return 0;
}


void setup_GPIO()
{
	ioremap = ioremap_nocache(GPIO_PC_BASE, 32);

	iowrite32(0x33333333, ioremap + GPIO_PC_MODEL);
	printk("Set pin 0-7 for input...\n");
 //GPIO_PC_MODEL = 0x33333333;
//DISWILLCRASH.PNG	iowrite32(0x000000ff, ioremap + GPIO_PC_DOUT);
//	printk("Enable internal pull-up...\n");
  //GPIO_PC_DOUT = 0xFF;
	iowrite32(0x22222222, ioremap + GPIO_EXTIPSELL);
	printk("Enable port C to handle the interrupt...\n");
  //GPIO_EXTIPSELL = 0x22222222;
	iowrite32(0x000000ff, ioremap + GPIO_EXTIFALL);
	printk("Set interrupt handling for 1->0 transitions...\n");
  //GPIO_EXTIFALL = 0xFF;
	iowrite32(0x000000ff, ioremap + GPIO_EXTIRISE);
	printk("Set interrupt handling for 0->1 transitions...\n");
	iowrite32(0x000000ff, ioremap + GPIO_IEN);
	printk("Enable interrupt generation...\n");
	printk("GPIO AND GPIO INTERRUPTS ARE NOW SET UP!\n");
  //GPIO_IEN = 0xFF;
}


void setup_interrupts()
{
	//request GPIO_EVEN IRQ line (nr17)
	err_even = request_irq(17, *handler, NULL, DEV_NAME, NULL);
	if(err_even){
		printk(KERN_INFO "can't get assigned irq 17\n");
	}
	//request GPIO_ODD IRQ line (nr18)
	err_odd = request_irq(18, *handler, NULL, DEV_NAME, NULL);
	if(err_odd){
		printk(KERN_INFO "can't get assigned irq 18\n");
	}
	if(!err_even && !err_odd){
		printk(KERN_INFO "IRQ set up successfully");
	}
}


/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	unregister_chrdev_region(0,2);
	release_mem_region(ioremap + GPIO_PC_DIN, 32);
	printk("Short life for a small module...\n");
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

