/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <efm32gg.h>

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

#define uint_32t *err;

static int __init template_init(void)
{
	printk("Hello World, here is your module speaking\n");
	*err = request_mem_region(GPIO_PC_BASE + GPIO_PC_DIN, 32, "GPIO IN");
	if (*err == NULL){
		printk("Failure");
	}
	else{
		printk("GPIO INPUT SUCCESS");
	}
	return 0;
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

