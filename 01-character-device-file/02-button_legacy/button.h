#ifndef __BUTTON_H__
#define __BUTTON_H__

/* GPIO address - size */
#define GPIO_0_ADDR_BASE    0x44E07000
#define GPIO_0_ADDR_SIZE	(0x44E07FFF - 0x44E07000)

/* Registers */
#define GPIO_OE_OFFSET			    0x134
#define GPIO_DATAOUT                0x13C
#define DEBOUNCEENABLE              0x150
#define GPIO_DEBOUNCINGTIME         0x154
#define GPIO_SETDATAOUT_OFFSET		0x194
#define GPIO_CLEARDATAOUT_OFFSET	0x190

/* GPIO Pin */
#define BTN     14       // P9_26 <=> GPIO_0_26 BUTTON
#define LED     31       // P9_1  <=> GPIO_0_31 LED
#define DEBOUNCING_VALUE    255

#endif  /*__BUTTON_H__ */
