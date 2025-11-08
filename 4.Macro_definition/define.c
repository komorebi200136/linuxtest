#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloworld_init(void) //驱动入口函数
{
#ifndef DEBUG
    printk("helloworld a!\n");
#else
    printk("helloworld b!\n");
#endif
    return 0;
}

static void __exit helloworld_exit(void) //驱动出口函数
{
	printk(KERN_EMERG "helloworld_exit\r\n");
}

module_init(helloworld_init); //注册入口函数
module_exit(helloworld_exit); //注册出口函数
MODULE_LICENSE("GPL v2"); //同意 GPL 开源协议
MODULE_AUTHOR("topeet"); //作者信息
