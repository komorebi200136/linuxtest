#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/types.h>

struct myobj {
	struct kobject *mykobject;
	int value1;
	int value2;
};
struct myobj test;
// 定义了一个 kobj_type 结构体变量 mytype，用于描述 kobject 的类型。
void myrelease(struct kobject *kobj)
{
	printk("kobject: (%p): %s\n", kobj, __func__);
	kfree(kobj);
}

struct attribute value1 = {
	.name = "value1",
	.mode = 0666,
};

struct attribute value2 = {
	.name = "value2",
	.mode = 0666,
};

struct attribute *mydefault_attrs[] = {
	&value1,
	&value2,
	NULL,
};

ssize_t	myshow(struct kobject *kobj, struct attribute *attr, char *buf)
{
	ssize_t count;
	printk("attrname: %s\n", attr->name);
	if (strcmp(attr->name, "value1") == 0) {
		count = sprintf(buf, "%d\n", test.value1);
	} else if (strcmp(attr->name, "value2") == 0) {
		count = sprintf(buf, "%d\n", test.value2);
	} else {
		count = 0;
	}
	return count;	
}

ssize_t	mystore(struct kobject *kobj, struct attribute *attr, const char *buf, size_t size)
{
	printk("attrname: %s\n", attr->name);
	if (strcmp(attr->name, "value1") == 0) {
		sscanf(buf, "%d\n", &test.value1);
	} else if (strcmp(attr->name, "value2") == 0) {
		sscanf(buf, "%d\n", &test.value2);
	}
	return size;
}

struct sysfs_ops mysysfs_ops = {
	.show = myshow,
	.store = mystore,
};

struct kobj_type mytype = {
	.release = myrelease,
	.sysfs_ops = &mysysfs_ops,
	.default_attrs = mydefault_attrs,	/* use default_groups instead */
};

static int __init mykobject_init(void)
{
	int ret;
	test.mykobject = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	ret = kobject_init_and_add(test.mykobject, &mytype, NULL, "%s","kobject01");
	if(ret<0) {
		printk(KERN_ERR "Failed to create kobject01\n");
		return -ENOMEM;
	}
	return 0;
}

static void __exit mykobject_exit(void)
{
	kobject_put(test.mykobject);
        printk("kobject exit\n");
}

module_init(mykobject_init);
module_exit(mykobject_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");