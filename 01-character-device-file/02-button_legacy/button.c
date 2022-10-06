#include <linux/module.h>	 /* This module defines functions such as module_init/module_exit */
#include <linux/gpio.h>		 /* For Legacy integer based GPIO */
#include <linux/interrupt.h> /* For IRQ */
#include <linux/delay.h>	 /* */
#include "button.h"

#define DRIVER_AUTHOR "thonv thonv@gmail.com"
#define DRIVER_DESC "Control LED with button"

static int irq;
uint32_t __iomem *base_addr;
volatile int32_t state;

static irqreturn_t btn_pushed_irq_handler(int irq, void *dev_id)
{
	state = gpio_get_value(LED);
	if (state == 0)
	{
		*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= (1 << LED);
		state = 1;
	}
	else
	{
		*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= (1 << LED);
		state = 0;
	}
	pr_info("BTN interrupt - LED state is: %d\n", state);
	return IRQ_HANDLED;
}

static int __init btn_init(void)
{
	uint8_t retval;

	/* Config LED as output mode*/
	base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
	if (!base_addr)
		return -ENOMEM;

	*(base_addr + GPIO_OE_OFFSET / 4) &= ~(1 << LED);
	*(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= (1 << LED);

	/* Config BTN as input mode */
	*(base_addr + GPIO_OE_OFFSET / 4) &= (1 << BTN);
	*(base_addr + DEBOUNCEENABLE / 4) &= (1 << BTN);
	*(base_addr + GPIO_DEBOUNCINGTIME / 4) &= DEBOUNCING_VALUE;

	irq = gpio_to_irq(BTN);
	retval = request_threaded_irq(irq, NULL,
								  btn_pushed_irq_handler,
								  IRQF_TRIGGER_LOW | IRQF_ONESHOT,
								  "BTN-sample", NULL);

	pr_info("Hello! Initizliaze successfully!\n");
	return 0;
}

static void __exit btn_exit(void)
{
	*(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= (1 << LED);
	free_irq(irq, NULL);
	iounmap(base_addr);

	pr_info("Good bye my fen !!!\n");
}

module_init(btn_init);
module_exit(btn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
