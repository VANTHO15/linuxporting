# Paths
BBB_KERNEL := /home/thonv/work/beagle_bone_black/bb-kernel/KERNEL/
TOOLCHAIN := /home/thonv/work/beagle_bone_black/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin

PREFIX := arm-linux-gnueabihf-
CC := gcc
LD := ld

all :
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN)/$(PREFIX) -C $(BBB_KERNEL) M=$(shell pwd) modules

clean :
	make -C $(BBB_KERNEL) M=$(shell pwd) clean