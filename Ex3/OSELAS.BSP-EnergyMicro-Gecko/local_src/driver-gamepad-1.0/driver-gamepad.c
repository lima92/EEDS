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
#include <linux/types.h> //?
#include <linux/fs.h>
#include <linux/device.h>

#include <asm/signal.h>
#include <asm/uaccess.h>

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

int setup_GPIO();
int setup_interrupts();

static int gp_open(struct inode*, struct file*);
static int gp_release(struct inode*, struct file*);
static ssize_t gp_read(struct file*, char* __user, size_t, loff_t*);
static ssize_t gp_write(struct file*, char* __user, size_t, loff_t*);

uint32_t *err, *err_odd, *err_even, *handler;
uint32_t *ioremap;


static struct class *gp_class;
static struct device *gp_device;
dev_t dev; //device number
struct cdev gp_cdev;
static int major;

static struct gamepad_dev{
	struct gamepad_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	unsigned int acces_key;
	struct semaphore sem;

} gp_dev;

struct file_operations gp_fops = {
	.owner = THIS_MODULE,
	.read = gp_read,
	.open = gp_open,
	.release = gp_release,
};




static int __init gamepad_init(void)
{
	printk("Hello World, here is your module: %c fucking v18\n", DEV_NAME);


	int err_reg = alloc_chrdev_region(&dev, 0, 1, DEV_NAME);
	printk("dev: %i\n",dev);

	//major = MAJOR(*dev);
	if(!err_reg){
		printk("CharDev reg successfull\n");
	}else{
		printk("Failed to register dev\n");
		printk("err: %i\n",err_reg);
		return err_reg;
	}
	

	cdev_init(&gp_cdev, &gp_fops);
	gp_cdev.owner = THIS_MODULE;
	printk(KERN_NOTICE "gp_dev.cdev.owner: %i", *gp_cdev.owner);
	int err_cdev = cdev_add(&gp_cdev, dev, 1);
	if(err_cdev){
		printk(KERN_NOTICE "Error %i adding dev%i", err_cdev, 0);
	}else{
		printk(KERN_NOTICE "cdev added successfully!\n");
	}

	

	//Create device file
	struct class *gp_class = class_create(THIS_MODULE, "gamepad-class");

	if(IS_ERR(gp_class)){
		return -1;
	}

	printk("Created classfile\n");
	struct device *devkok = device_create(gp_class, NULL, dev, NULL, DEV_NAME);
	
	pr_err("%s:%d error code %d\n", __func__, __LINE__, PTR_ERR(devkok));

	err = request_mem_region(GPIO_PC_BASE, 4, DEV_NAME);
	
	if (*err == NULL){
		printk("Failure\n");
		return -1;
	}
	else{
		printk("GPIO INPUT SUCCESS\n");
	}

	int err_gpio = setup_GPIO();
	int err_irq = setup_interrupts();
	return 0;
}

static int gp_open(struct inode *inode, struct file *file)
{
	printk("Opening Gamepad driver..\n\n");
	
	
	return 0;
}
		
static int gp_release(struct inode* inode, struct file* file)
{
	printk("Closing Gamepad driver..\n\n");
}

int setup_GPIO(void)
{
	//ioremap = GPIO_PC_BASE; //ioremap_nocache(GPIO_PC_BASE, 32);

	iowrite32(0x33333333,  GPIO_PC_MODEL);
	printk("Set pin 0-7 for input...\n");
	printk("MODEL: %i\n", ioread32(GPIO_PC_MODEL));
 //GPIO_PC_MODEL = 0x33333333;
	iowrite32(0xff,  GPIO_PC_DOUT);
	printk("Enable internal pull-up...\n");
  //GPIO_PC_DOUT = 0xFF;
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	printk("Enable port C to handle the interrupt...\n");
  //GPIO_EXTIPSELL = 0x22222222;
	iowrite32(0x000000ff,  GPIO_EXTIFALL);
	printk("Set interrupt handling for 1->0 transitions...\n");
  //GPIO_EXTIFALL = 0xFF;
	iowrite32(0x000000ff,  GPIO_EXTIRISE);
	printk("Set interrupt handling for 0->1 transitions...\n");
	iowrite32(0x000000ff,  GPIO_IEN);
	printk("Enable interrupt generation...\n");
	printk("GPIO AND GPIO INTERRUPTS ARE NOW SET UP!\n");
  //GPIO_IEN = 0xFF;
	return 0;
}


int setup_interrupts(void)
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
		return 0;
	}else{
		return -1;
	}
	
}


static ssize_t gp_read(struct file* file, char* __user buff, size_t count, loff_t* loff_t)
{
	printk("Reading Gamepad..\n");
	uint16_t val = ioread16(GPIO_PC_DIN);
	printk("Val = %i\n", val);
	int cp_err = copy_to_user(buff, &val, 1);
	printk(KERN_INFO "cp_err: %i\n", cp_err);
	if(cp_err){
		return cp_err;
	}else{
		return 1;
	} 
}

static irqreturn_t gpio_interrupt_handler();

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	unregister_chrdev_region(0,2);
	release_mem_region(GPIO_PC_DIN, 32);
	printk("Short life for a small module...\n");
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

