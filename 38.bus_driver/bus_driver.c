#include "linux/device/bus.h"
#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/device.h>

extern struct bus_type mybus;

int mydriver_probe (struct device *dev)
{
	printk(KERN_INFO "mydriver_probe: %s\n", dev_name(dev));
	return 0;
}

int mydriver_remove (struct device *dev)
{
	printk(KERN_INFO "mydriver_remove: %s\n", dev_name(dev));
	return 0;
}

struct device_driver mydriver = {
	.name = "mydevice", // 驱动程序的名称
	.bus = &mybus, // 驱动程序所属的总线类型
	.probe = mydriver_probe, // 驱动程序的 probe 函数
	.remove = mydriver_remove, // 驱动程序的 remove 函数
};

// 模块的初始化函数
static int mydriver_init(void)
{
	int ret;
	ret = driver_register(&mydriver); // 注册设备
	if (ret) {
		printk(KERN_ERR "device_register failed\n");
		return ret;
	}
	return 0;
}

// 模块退出函数
static void mydriver_exit(void)
{
	driver_unregister(&mydriver); // 取消注册设备
}
module_init(mydriver_init);
module_exit(mydriver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");