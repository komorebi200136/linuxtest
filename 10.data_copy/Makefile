export ARCH=arm64
export CROSS_COMPILE=/home/ubuntu20/linux/kernel_5.1/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
obj-m +=  data_copy.o
KDIR := /home/ubuntu20/linux/kernel_5.1/kernel
PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean