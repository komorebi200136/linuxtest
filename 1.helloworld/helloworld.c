#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloInit(void)
{
    printk("hello world");
    return 0;
}

static void __exit helloExit(void)
{
    printk("good bye, cruel world");
}

module_init(helloInit);
module_exit(helloExit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");