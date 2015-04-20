/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/interrupt.h>

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

static const char *dev_name = "GAMEPAD";

void setup_GPIO();
void setup_interrupts();

uint32_t *err, *err_odd, *err_even, *handler;
uint32_t *ioremap;

static int __init template_init(void)
{
	printk("Hello World, here is your module fucking\n");
	err = request_mem_region(GPIO_PC_BASE + GPIO_PC_DIN, 32, *dev_name);
	ioremap = ioremap_nocache(GPIO_PC_BASE + GPIO_PC_DIN, 32);
	if (*err == NULL){
		printk("Failure\n");
		return -1;
	}
	else{
		printk("GPIO INPUT SUCCESS\n");
	}

	setup_GPIO();

	setup_interrupts();
	return 0;
}

void setup_GPIO()
{
	iowrite32(0x33333333, ioremap + GPIO_PC_MODEL);
	printk("Set pin 0-7 for input...");
  //GPIO_PC_MODEL = 0x33333333;
	iowrite32(0xff, ioremap + GPIO_PC_DOUT);
	printk("Enable internal pull-up...");
  //GPIO_PC_DOUT = 0xFF;
	iowrite32(0x22222222, ioremap + GPIO_EXTIPSELL);
	printk("Enable port C to handle the interrupt...");
  //GPIO_EXTIPSELL = 0x22222222;
	iowrite32(0xff, ioremap + GPIO_EXTIFALL);
	printk("Set interrupt handling for 1->0 transitions...");
  //GPIO_EXTIFALL = 0xFF;
	iowrite32(0xff, ioremap + GPIO_EXTIRISE);
	printk("Set interrupt handling for 0->1 transitions...");
	iowrite32(0xff, ioremap + GPIO_IEN);
	printk("Enable interrupt generation...");
	printk("GPIO AND GPIO INTERRUPTS ARE NOW SET UP!");
  //GPIO_IEN = 0xFF;
}


void setup_interrupts()
{
	err_even = request_irq(17, *handler, NULL,*dev_name, NULL);
	if(err_even){
		printk(KERN_INFO "can't get assigned irq 17\n");
	}
	err_odd = request_irq(18, *handler, NULL,*dev_name, NULL);
	if(err_odd){
		printk(KERN_INFO "can't get assigned irq 18\n");
	}
}


/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	release_mem_region(ioremap + GPIO_PC_DIN, 32);
	printk("Short life for a small module...\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

