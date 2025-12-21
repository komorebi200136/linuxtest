#include "linux/device/bus.h"
#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/device.h>

static ssize_t myattr_show(struct bus_type *bus, char *buf)
{
	return sprintf(buf, "myattr_show\n");
}

static ssize_t myattr_store(struct bus_type *bus, const char *buf, size_t count)
{
	printk("myattr_store: %s\n", buf);
	return count;
}

struct bus_attribute attr = {
	.attr = {
		.name = "myattr",
		.mode = S_IRUGO | S_IWUSR,
	},
	.show = myattr_show,
	.store = myattr_store,
};

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
EXPORT_SYMBOL_GPL(mybus); // 新加导出总线符号

static int __init mybus_init(void)
{
	int ret = 0;
	printk("bus_create: initializing module\n");
	ret = bus_register(&mybus);
	if (ret) {
		printk("bus_create: failed to register bus\n");
		return ret;
	}
	ret =bus_create_file(&mybus, &attr);
	if (ret) {
		printk("bus_create: failed to create sysfs file\n");
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