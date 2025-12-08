#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>


static int my_platform_probe(struct platform_device *pdev)
{
	struct resource *res_irq, *res_mem;	
	res_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res_irq) {
		printk(KERN_ERR "my_platform_probe: No IRQ resource defined\n");
		return -ENODEV;
	}
	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res_mem) {
		printk(KERN_ERR "my_platform_probe: No memory resource defined\n");
		return -ENODEV;
	}
	printk(KERN_INFO "my_platform_probe: Resource start = %lld, end = %lld\n", res_mem->start, res_mem->end);
	printk(KERN_INFO "my_platform_probe: Resource start = %lld, end = %lld\n", res_irq->start, res_irq->end);
	return 0;
}

static int my_platform_remove(struct platform_device *pdev)
{
	printk(KERN_INFO "my_platform_remove: Removing platform device\n");
	return 0;
}

struct platform_device_id myplatform_id_table[] = {
	{"myplatform_device", 0},
	{ },
};

struct platform_driver my_platform_driver = {
	.probe = my_platform_probe,
	.remove = my_platform_remove,
	.driver = {
		.name = "myplatform_device",
		.owner = THIS_MODULE,
	},
	.id_table = myplatform_id_table,
};

static int __init platform_init(void)
{
	int ret;
	ret = platform_driver_register(&my_platform_driver);
	if (ret) {
		printk(KERN_ERR "platform_driver_register failed\n");
		return ret;
	}
        printk("platform init\n");
        return 0;
}

static void __exit platform_exit(void)
{
	platform_driver_unregister(&my_platform_driver);
        printk("platform exit\n");
}

module_init(platform_init);
module_exit(platform_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");