        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
	      //load CMU base address
		ldr r1, cmu_base_addr

		//load current value of HFPERCLK ENABLE
		ldr r2, [r1, #CMU_HFPERCLKEN0]

		//set bit for GPIO clk
		mov r3, #1
		lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		orr r2, r2, r3

		str r2, [r1, #CMU_HFPERCLKEN0]

		//load GPIO_CTRL addr
		ldr r3,  gpio_pa_base_addr
		mov r4, #0x2
		str r4, [r3]

		
		//set pins 8-15 to output
		//ldr r3, gpio_pa_base_addr
		mov r4, #0x55555555
		str r4, [r3, #GPIO_MODEH]

		//set LEDs
		mov r4, #0xFF00
		str r4, [r3, #GPIO_DOUT]

		//set pins for input
		ldr r5, gpio_pc_base_addr
		mov r6, #0x33333333
		str r6, [r5, #GPIO_MODEL]

		//enable internal pull-up
		mov r6, #0xFF
		str r6, [r5, #GPIO_DOUT]

		//interrupt
		ldr r8, gpio_base_addr
		mov r6, #0x22222222
		str r6, [r8]

		//enable interrupt on falling edge
		mov r6, #0xFF
		str r6, [r8, #GPIO_EXTIRISE]
		str r6, [r8, #GPIO_EXTIFALL]
		str r6, [r8, #GPIO_IEN]

		//Enable odd and even interrupt for GPIO
		ldr r2, iser0_addr
		ldr r6, =#0x0802
		str r6, [r2]
		
		//enable deepsleep
 		ldr r2, scr_addr
		ldr r6, =#0x6
		str r6, [r2]

		//set counter to zero
		mov r0, #0b11111111
		bl update_leds
		b main
	
	/////////////////////////////////////////////////////////////////////////////
	//
	
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:
		//clear interrupt
		ldr r6, [r8, #GPIO_IF]
		str r6, [r8, #GPIO_IFC]
	
		//read buttons
		ldr r7, [r5, #GPIO_DIN]
	
		//check if sw8 is pressed
		cmp r7, #0b01111111
		beq decrement
	
		//check if sw6 is pressed
		cmp r7, #0b11011111
		beq increment

		bx lr
	
	/////////////////////////////////////////////////////////////////////////////

//main loop
main:
		wfi
		bl main


increment:
		//subtract 1 from the counter variable
		sub r0, r0, #1
		b update_leds

decrement:
		//add 1 from the counter variable
		add r0, r0, #1
		b update_leds
		

update_leds:
		//left shift and store the value on GPIO_DOUT
		lsl r9, r0, #8
		str r9, [r3, #GPIO_DOUT]
		bx lr

        .thumb_func
dummy_handler:  
        b .  // do nothing

cmu_base_addr:
	.long CMU_BASE

gpio_base_addr:
	.long GPIO_BASE

gpio_pa_base_addr:
	.long GPIO_PA_BASE

gpio_pc_base_addr:
	.long GPIO_PC_BASE

iser0_addr:
	.long ISER0

scr_addr:
	.long SCR

