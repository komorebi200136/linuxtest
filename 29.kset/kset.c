#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/configfs.h>

struct kobject *kobject01;
struct kobject *kobject02;
struct kset *mykset;
// 定义了一个 kobj_type 结构体变量 mytype，用于描述 kobject 的类型。
struct kobj_type mytype;
static int __init mykset_init(void)
{
	int ret;
	mykset =kset_create_and_add("mykset", NULL, NULL);
	if (!mykset) {
		printk(KERN_ERR "Failed to create mykset\n");
		return -ENOMEM;
	}

	kobject01 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	kobject01->kset = mykset;
	ret = kobject_init_and_add(kobject01, &mytype, NULL, "%s","kobject01");
	if(ret<0) {
		printk(KERN_ERR "Failed to create kobject01\n");
		return -ENOMEM;
	}
	kobject02 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	kobject02->kset = mykset;
	ret = kobject_init_and_add(kobject02, &mytype, NULL, "%s","kobject02");
	if(ret<0) {
		printk(KERN_ERR "Failed to create kobject02\n");
		return -ENOMEM;
	}
	return 0;
}

static void __exit mykset_exit(void)
{
	kobject_put(kobject01);
	kobject_put(kobject02);
	kset_unregister(mykset);

        printk("kobject exit\n");
}

module_init(mykset_init);
module_exit(mykset_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");