#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

extern int mathadd(int a, int b);
static int __init mathinit(void)
{
    printk("mathadd: %d", mathadd(1, 2));
    printk("mathinit");
    return 0;
}

static void __exit mathexit(void)
{
    printk("mathexit");
}

module_init(mathinit);
module_exit(mathexit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");