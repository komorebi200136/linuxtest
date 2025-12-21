#include "linux/device/bus.h"
#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/device.h>

extern struct bus_type mybus;

void myrelease(struct device *dev)
{
	printk("This is myrelease\n");
};

struct device mydevice = {
	.init_name = "mydevice", // 设备的初始化名称
	.bus = &mybus, // 所属总线
	.release = myrelease, // 设备的释放回调函数
	.devt = MKDEV(255, 0), // 设备号
};

// 模块的初始化函数
static int device_init(void)
{
	int ret;
	ret = device_register(&mydevice); // 注册设备
	if (ret) {
		printk(KERN_ERR "device_register failed\n");
		return ret;
	}
	return 0;
}

// 模块退出函数
static void device_exit(void)
{
	device_unregister(&mydevice); // 取消注册设备
}
module_init(device_init);
module_exit(device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");