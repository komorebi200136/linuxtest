#include "linux/device/bus.h"
#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/device.h>

int mybus_match(struct device *dev, struct device_driver *drv)
{
	return strcmp(dev_name(dev), drv->name);
}

int mybus_probe(struct device *dev)
{
	struct device_driver *drv = dev->driver;
	if (drv->probe)
		drv->probe(dev);
	return 0;
}
struct bus_type mybus = {
	.name = "mybus",
	.match = mybus_match,
	.probe = mybus_probe,
};
static int __init mybus_init(void)
{
	int ret = 0;
	printk("bus_create: initializing module\n");
	ret = bus_register(&mybus);
	if (ret) {
		printk("bus_create: failed to register bus\n");
		return ret;
	}
	return 0;
}

static void __exit mybus_exit(void)
{
	printk("bus_create: unloading module\n");
	bus_unregister(&mybus);
}

module_init(mybus_init);
module_exit(mybus_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");