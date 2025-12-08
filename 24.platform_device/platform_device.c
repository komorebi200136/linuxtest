#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

struct resource myplatform_resource[] = {
	{
		.start = 0x10000000,
		.end = 0x10000fff,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = 42,
		.end = 42,
		.flags = IORESOURCE_IRQ,
	},
};



void myplatform_device_release(struct device *dev)
{

}

struct platform_device myplatform_device = {
	.name = "myplatform_device",
	.id = -1,
	.num_resources = ARRAY_SIZE(myplatform_resource),
	.resource = myplatform_resource,
	.dev.release = myplatform_device_release,
};

static int __init platform_init(void)
{
	platform_device_register(&myplatform_device);
        printk("platform init\n");
        return 0;
}

static void __exit platform_exit(void)
{
	platform_device_unregister(&myplatform_device);
        printk("platform exit\n");
}

module_init(platform_init);
module_exit(platform_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");