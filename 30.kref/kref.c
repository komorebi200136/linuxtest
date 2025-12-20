#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>

struct kobject *kobject01;
struct kobject *kobject02;
struct kobject *kobject03;

// 定义了一个 kobj_type 结构体变量 mytype，用于描述 kobject 的类型。
struct kobj_type mytype;
static int __init mykobject_init(void)
{
	int ret;
	//创建kobject的第一种方法
	kobject01 = kobject_create_and_add("kobject01", NULL);
	if (!kobject01) {
		printk(KERN_ERR "Failed to create kobject01\n");
		return -ENOMEM;
	}
	kobject02 = kobject_create_and_add("kobject02", kobject01);
	if (!kobject02) {
		printk(KERN_ERR "Failed to create kobject02\n");
		return -ENOMEM;
	}
	//创建kobject的第二种方法
	kobject03 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	ret = kobject_init_and_add(kobject03, &mytype, NULL, "%s","kobject03");
	if(ret<0) {
		printk(KERN_ERR "Failed to create kobject03\n");
		return -ENOMEM;
	}
	printk("kobject01=%d\n",kobject01->kref.refcount.refs.counter);
	printk("kobject02=%d\n",kobject02->kref.refcount.refs.counter);
	printk("kobject03=%d\n",kobject03->kref.refcount.refs.counter);
	return 0;
}

static void __exit mykobject_exit(void)
{
	kobject_put(kobject01);
	printk("kobject01=%d\n",kobject01->kref.refcount.refs.counter);
	printk("kobject02=%d\n",kobject02->kref.refcount.refs.counter);
	printk("kobject03=%d\n",kobject03->kref.refcount.refs.counter);
	kobject_put(kobject02);
	printk("kobject01=%d\n",kobject01->kref.refcount.refs.counter);
	printk("kobject02=%d\n",kobject02->kref.refcount.refs.counter);
	printk("kobject03=%d\n",kobject03->kref.refcount.refs.counter);
	kobject_put(kobject03);
	printk("kobject01=%d\n",kobject01->kref.refcount.refs.counter);
	printk("kobject02=%d\n",kobject02->kref.refcount.refs.counter);
	printk("kobject03=%d\n",kobject03->kref.refcount.refs.counter);
        printk("kobject exit\n");
}

module_init(mykobject_init);
module_exit(mykobject_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");