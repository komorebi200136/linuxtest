#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>

struct kobject *mykobject;

// 定义了一个 kobj_type 结构体变量 mytype，用于描述 kobject 的类型。
void myrelease(struct kobject *kobj)
{
	printk("kobject: (%p): %s\n", kobj, __func__);
	kfree(kobj);
}

struct kobj_type mytype = {
	.release = myrelease,
};

static int __init mykobject_init(void)
{
	int ret;
	mykobject = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	ret = kobject_init_and_add(mykobject, &mytype, NULL, "%s","kobject03");
	if(ret<0) {
		printk(KERN_ERR "Failed to create kobject03\n");
		return -ENOMEM;
	}
	return 0;
}

static void __exit mykobject_exit(void)
{
	kobject_put(mykobject);
        printk("kobject exit\n");
}

module_init(mykobject_init);
module_exit(mykobject_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");