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
#include <asm/siginfo.h>
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

struct resource *gpio_pc;
struct resource *gpio_irq;
int setup_GPIO();
int setup_interrupts();

//static irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);

static int gp_open(struct inode*, struct file*);
static int gp_release(struct inode*, struct file*);
static ssize_t gp_read(struct file*, char* __user, size_t, loff_t*);
static int gp_fasync(int, struct file*, int);

uint32_t *err, *err_odd, *err_even, *handler;

void *ioremap;
void *irq_remap;

static struct class *gp_class;
static struct device *gp_device;
dev_t dev; //device number
struct cdev gp_cdev;
struct fasync_struct *fasync;
static int major;

static struct gamepad_dev{
	struct gp_qset *data;
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
	.fasync = gp_fasync,
};

irqreturn_t gpio_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	//printk("Handling GPIO interrupt..\n");
	//clear interrupt flag
	iowrite16(ioread16(irq_remap + GPIO_IF - GPIO_EXTIPSELL), irq_remap + GPIO_IFC - GPIO_EXTIPSELL);
	uint16_t button_state = ioread16(ioremap + GPIO_PC_DIN);
	//Send signal
	if(fasync){
		kill_fasync(&fasync, SIGIO, POLL_IN);
	}
	//printk("Button state: %i\n", button_state);
	return IRQ_HANDLED;
}

static int gp_fasync(int fd, struct file *filp, int mode)
{
	//printk("HERRO DIS IS FASYNC\n");
	struct gamepad_dev *gp_dev = filp->private_data;

	return fasync_helper(fd, filp, mode, &fasync);
}

//INIT gamepad
static int __init gamepad_init(void)
{
	printk("Hello World, here is your module: %c speaking v18\n", DEV_NAME);

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

	int err_gpio = setup_GPIO();
	int err_irq = setup_interrupts();
	return 0;
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

//GPIO setup
int setup_GPIO(void)
{
	gpio_pc = request_mem_region(GPIO_PC_BASE, 36, DEV_NAME);
	
	if (IS_ERR(gpio_pc)){
		printk("Failure\n");
		return -1;
	}
	else{
		printk("GPIO INPUT SUCCESS\n");
	}

	//ioremap = GPIO_PC_BASE; 
	ioremap = ioremap_nocache(GPIO_PC_BASE, 36);
	if(IS_ERR(ioremap)){return -1;}
	
	iowrite32(0x33333333, ioremap + GPIO_PC_MODEL);
	printk("Set pin 0-7 for input...\n");
	printk("MODEL: %i\n", ioread32(ioremap + GPIO_PC_MODEL));
 	//GPIO_PC_MODEL = 0x33333333;
	iowrite32(0xff, ioremap + GPIO_PC_DOUT);
	printk("Enable internal pull-up...\n");
  	//GPIO_PC_DOUT = 0xFF;

	printk("GPIO ARE NOW SET UP!\n\n");
  	//GPIO_IEN = 0xFF;
	return 0;
}


int setup_interrupts(void)
{
	gpio_irq = request_mem_region(GPIO_PC_BASE + GPIO_EXTIPSELL, 32, DEV_NAME);
	
	if (IS_ERR(gpio_irq)){
		printk("Failure\n");
		return -1;
	}
	else{
		printk("IRQ MEM REQUEST SUCCESS\n");
	}

	//ioremap = GPIO_PC_BASE; 
	irq_remap = ioremap_nocache(GPIO_PA_BASE + GPIO_EXTIPSELL, 32);
	if(IS_ERR(irq_remap)){return -1;}

  	//GPIO_EXTIPSELL = 0x22222222;
	iowrite32(0x22222222, irq_remap);
	printk("Enable port C to handle the interrupt...\n%i\n", ioread32(irq_remap));

	//GPIO_EXTIFALL = 0xFF;
	iowrite32(0xff,  irq_remap + GPIO_EXTIFALL - GPIO_EXTIPSELL);
	printk("Set interrupt handling for 1->0 transitions...\n%i\n", ioread32(irq_remap + GPIO_EXTIFALL - GPIO_EXTIPSELL));

	//GPIO_EXTIRISE = 0xFF;
	iowrite32(0xff,  irq_remap + GPIO_EXTIRISE - GPIO_EXTIPSELL);
	printk("Set interrupt handling for 0->1 transitions...\n%i\n", ioread32(irq_remap + GPIO_EXTIRISE - GPIO_EXTIPSELL));

	//request GPIO_EVEN IRQ line (nr17)
	err_even = request_irq(17, (irq_handler_t)gpio_interrupt_handler, 0, DEV_NAME, &gp_cdev);
	if(err_even){
		printk(KERN_INFO "can't get assigned irq 17\n");
	}

	//request GPIO_ODD IRQ line (nr18)
	err_odd = request_irq(18, (irq_handler_t)gpio_interrupt_handler, 0, DEV_NAME, &gp_cdev);
	if(err_odd){
		printk(KERN_INFO "can't get assigned irq 18\n");
	}

	//GPIO_IEN = 0xFF;
	iowrite32(0xff,  irq_remap + GPIO_IEN - GPIO_EXTIPSELL);
	printk("Enable interrupt generation...\n%i\n", ioread32(irq_remap + GPIO_IEN - GPIO_EXTIPSELL));

	if(!err_even && !err_odd){
		printk(KERN_INFO "GPIO INTERRUPTS ARE NOW SET UP!\n\n");
		return 0;
	}else{
		return -1;
	}
	
}


static ssize_t gp_read(struct file* file, char* __user buff, size_t count, loff_t* loff_t)
{
	printk("Reading Gamepad..\n");
	uint16_t val = ioread16(ioremap + GPIO_PC_DIN);
	printk("Val = %i\n", val);
	int cp_err = copy_to_user(buff, &val, count);
	if(cp_err){
		return cp_err;
		printk(KERN_INFO "cp_err: %i\n", cp_err);
	}else{
		return count;
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
	unregister_chrdev_region(0,1);
	release_mem_region(ioremap, 36);
	printk("Short life for a small module...\n");
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

