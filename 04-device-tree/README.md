
# **Device Tree**
## Introduction to the device tree
## Device tree structure
## Example of a device node
## Properites of device trees
## Device tree overlays

### **I. Introduction to the device tree**
- Used for hardware description data with software or OS.
- It is a description of hardware that is readable by an operating system so that the operating system doesn't need to hard code details of machine

Source: Documentation/devicetree/usage-model.txt

- Linux uses DT for, 
  - Platform indetification
  - Device population
		The kernel parses the device tree data and generates the required software data structure, which will be used by the kernel code.
	
- Ideally, the device tree is indenpendent of any os; when you change the OS, you can still use the same DT file to describe the HW to the new OS. 
- That is, the DT makes "adding of device information" indenpendent of OS.
		
### **II. Device tree structure**

- The DT supports a hierachical way of writing HW description at the, 
  - SOC level	(am33xx.dtsi)
		On-chip processor and on-chip peripherals
  - Common board level (am335x-bone-common.dtsi)
		Peripherals onboard, like sensors, LEDs, button, external memory 
  - Board-specific level	(am335x-boneblack.dts)
  
#### 2.1 Property
- name: <name>[@address]
- Alias, lable, phandle
- address cell
- size cell

### 2.2 API
- Register access
>   struct resource *res; \
    void __iomem *base; \
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0); \
    base = devm_ioremap_resource(&pdev->dev, res); \

- Get other specific data of a device.
    - String
    >    mproperty-string = "string"    \
                                        \
        const char *my_string = NULL;   \
        of_property_read_string(pdev->dev.of_node, "string-property", &my_string);  \

    - 32-bit integers and arrays
    >   int-property = <197>; \
        int-list-property = <1350000 0x54dae47 1250000 1200000>; \
                                                                \
        int of_property_read_u32(const struct device_node *np, const char *propname, u32 index, u32 *out_value) \
        int of_property_read_u32_array(const struct device_node *np, const char *propname, u32 *out_values, size_tsz); \
                                                                \
        Ex1: \
        unsigned int number;    \
        of_property_read_u32(pdev->dev.of_node, "one-cell-property", &number); \
            \         
        Ex2:  \
        unsigned int cells_array[4];    \
        if (of_property_read_u32_array(pdev->dev.of_node,"int-list-property",cells_array, 4)) { \
            dev_err(&pdev->dev, "list of cells not specified\n"); \
            return -EINVAL; \
        }
    - Boolean
    > bool my_bool = of_property_read_bool(pdev->dev.of_node,"boolean-property");

### **III. The descriptor-based GPIO interface – the new and recommended way**
- A GPIO is characterized by a struct gpio_desc structure
		
>	#include <linux/gpio/consumer.h> \
                                \
	struct gpio_desc {          \
		struct gpio_chip *chip; \
		unsigned long flags;    \
		const char *label;      \
	};
	
- With the descriptor-based interface, prior to allocating and taking the ownership of GPIOs, those GPIOs must have been mapped somewhere. Actually, there are three kinds of
mapping in the kernel:
  - Platform data mapping: The mapping is done in the board file.
  - Advanced Configuration and Power Interface mapping (ACPI): The mapping is done in ACPI style. Generally used on x86-based systems.
  - Device tree: The mapping is done in DT style.

#### 3.1 GPIO descriptor mapping - the device tree

- property: <name>-gpios or <name>-gpio 
- Every descriptor-based interface function relies on the gpio_suffixes[] variable, defined in drivers/gpio/gpiolib.h and shown as follows:
	
>	/* gpio suffixes used for ACPI and device tree lookup */            \
	static const char * const gpio_suffixes[] = { "gpios", "gpio" };    \

#### 3.2 API
- Allocate and using gpio
> struct gpio_desc *gpiod_get_index(struct device *dev, const char *con_id, unsigned int idx, enum gpiod_flags flags) \
> struct gpio_desc *gpiod_get(struct device *dev, const char *con_id, enum gpiod_flags flags) \
> int gpiod_direction_input(struct gpio_desc *desc); \
> int gpiod_direction_output(struct gpio_desc *desc, int value); \
> int gpiod_get_value(const struct gpio_desc *desc); \
> void gpiod_set_value(struct gpio_desc *desc, int value); \
> int gpiod_put(struct gpio_desc *desc);
> int gpiod_set_debounce(struct gpio_desc *desc, unsigned debounce); \
> int gpiod_to_irq(const struct gpio_desc *desc); \
> int gpiod_cansleep(const struct gpio_desc *desc); 

	