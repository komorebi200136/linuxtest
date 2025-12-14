#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>

/** 
* 1.GPIO_ACTIVE_LOW,低电平有效，1为低电平，0为高电平，GPIO_ACTIVE_HIGH，1为高电平，0为低电平
* 2.gpiod_flags可以决定初始状态，输出高电平还是低电平，还是设置成输入状态 
* 3.pinctrl的pcfg_pull_none、pcfg_pull_up、pcfg_pull_down可以设置上拉、下拉、不拉电阻
* 但是这个上拉驱动能力很弱，灯很暗
* 4.pinctrl的上拉/下拉是硬件电阻,gpiod的方向/电平是软件控制
*/
static int my_platform_probe(struct platform_device *pdev)
{
	struct gpio_desc *led;
	struct device_node *node = pdev->dev.of_node;
	if (!node) {
		dev_err(&pdev->dev, "No device tree node found\n");
		return -ENODEV;
    	}
	// node = of_find_node_by_name(NULL, "led_dts");
	// if (!node) {
	// 	printk(KERN_ERR "can't find led_dts node\n");
	// 	return -ENODEV;
	// }
	printk("node name: %s\n", node->name);
	led =devm_gpiod_get_optional(&pdev->dev, "led", GPIOD_IN); 
	/**
	* Optional flags that can be passed to one of gpiod_* to configure direction
	* and output value. These values cannot be OR'd.
	*/
	// enum gpiod_flags {
	// 	GPIOD_ASIS	= 0,
	// 	GPIOD_IN	= GPIOD_FLAGS_BIT_DIR_SET,
	// 	GPIOD_OUT_LOW	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT,
	// 	GPIOD_OUT_HIGH	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT |
	// 			GPIOD_FLAGS_BIT_DIR_VAL,
	// 	GPIOD_OUT_LOW_OPEN_DRAIN = GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_OPEN_DRAIN,
	// 	GPIOD_OUT_HIGH_OPEN_DRAIN = GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_OPEN_DRAIN,
	// };
	if (IS_ERR(led)) {
		printk(KERN_ERR "can't get led gpio\n");
		return PTR_ERR(led);
	}
	printk(KERN_INFO "led probe\n");

	// mdelay(1000); 
	// ///gpiod_set_value(led, 1);	// 点亮 这设置GPIO的输出值为1（逻辑值），但要求GPIO必须已经是输出方向   
	gpiod_direction_output(led, 1);//可以设置成GPIOD_IN，将GPIO设置为输出方向，并设置初始输出值为1	
	// mdelay(1000);             		// 延迟1秒
	// //gpiod_set_value(led, 0);	// 熄灭
	gpiod_direction_output(led, 0);//可以设置成GPIOD_IN，将GPIO设置为输出方向，并设置初始输出值为1
	return 0;
}

static int my_platform_remove(struct platform_device *pdev)
{
	printk(KERN_INFO "led remove\n");
	return 0;
}

struct of_device_id myplatform_id_table[] = {
	{.compatible = "led_dts",},
	{ },
};

struct platform_driver my_platform_driver = {
	.probe = my_platform_probe,
	.remove = my_platform_remove,
	.driver = {
		.name = "test",
		.owner = THIS_MODULE,
		.of_match_table = myplatform_id_table,
	},
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