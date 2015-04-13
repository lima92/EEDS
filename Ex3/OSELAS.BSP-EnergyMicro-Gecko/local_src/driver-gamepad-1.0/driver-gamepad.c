/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>

#include <efm32gg.h>
#include <stdint.h>
#include <stdbool.h>
#include <buttons.h>

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

#define uint_32t *err;
#define uint_32t *ioremap;

static int __init template_init(void)
{
	printk("Hello World, here is your module fucking\n");
	*err = request_mem_region(GPIO_PC_BASE + GPIO_PC_DIN, 32, "GPIO IN");
	*ioremap = ioremap_nocache(GPIO_PC_BASE + GPIO_PC_DIN, 32);
	if (*err == NULL){
		printk("Failure\n");
	}
	else{
		printk("GPIO INPUT SUCCESS\n");
	}
	return 0;
}

void setup_GPIO()
{
  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xFF;

  *GPIO_EXTIPSELL = 0x22222222;
  *GPIO_EXTIFALL = 0xFF;
  *GPIO_IEN = 0xFF;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	release_mem_region(GPIO_PC_BASE + GPIO_PC_DIN, 32);
	printk("Short life for a small module...\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

