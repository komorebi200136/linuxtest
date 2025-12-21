#include "linux/gpio/consumer.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>

struct kobject *kobject01;

// 自定义的 show 函数，用于读取属性值
ssize_t show_attr01(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	ssize_t count;
	count = sprintf(buf, "show_myvalue1\n");
	return count;
};
// 自定义的 store 函数，用于写入属性值
ssize_t store_attr01(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk("buf is %s\n", buf);
	return count;
};

// 自定义的 show 函数，用于读取属性值
ssize_t show_attr02(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	ssize_t count;
	count = sprintf(buf, "show_myvalue2\n");
	return count;
};
// 自定义的 store 函数，用于写入属性值
ssize_t store_attr02(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk("buf is %s\n", buf);
	return count;
};

struct kobj_attribute value1 = __ATTR(attr01, 0664, show_attr01, store_attr01);
struct kobj_attribute value2 = __ATTR(attr02, 0664, show_attr02, store_attr02);

struct attribute *myattrs[] = {
	&value1.attr,
	&value2.attr,
	NULL,
};

struct attribute_group mygroups = {
	.name = "mygroups",
	.attrs = myattrs,
};

static int __init mykobject_init(void)
{
	int ret;
	//创建kobject的第一种方法
	kobject01 = kobject_create_and_add("kobject01", NULL);
	if (!kobject01) {
		printk(KERN_ERR "Failed to create kobject01\n");
		return -ENOMEM;
	}
	ret = sysfs_create_group(kobject01, &mygroups);
	if (ret) {
		printk(KERN_ERR "Failed to create sysfs group for kobject01\n");
		return ret;
	}
	printk("kobject init\n");
	return 0;
}

static void __exit mykobject_exit(void)
{
	kobject_put(kobject01);
        printk("kobject exit\n");
}

module_init(mykobject_init);
module_exit(mykobject_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");