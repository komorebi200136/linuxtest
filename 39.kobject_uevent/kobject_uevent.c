#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
struct kobject *mykobject01;
struct kset *mykset;
struct kobj_type mytype;

/*
直接使用kobject_create_and_add创建并添加一个 kobject，并触发一个 uevent 事件，会触发失败
只使用kobject_init_and_add，用户空间接收不到change事件
*/
static int mykobj_init(void)
{
	int ret;
	// 创建并添加一个 kset
	mykset = kset_create_and_add("mykset", NULL, NULL);
	// 分配并初始化一个 kobject
	mykobject01 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	mykobject01->kset = mykset;
	// 初始化并添加 kobject 到 kset
	ret = kobject_init_and_add(mykobject01, &mytype, NULL, "%s", "mykobject01");
	// 触发一个 uevent 事件，表示 kobject 的属性发生了变化
	ret = kobject_uevent(mykobject01, KOBJ_CHANGE);
	return 0;
}

static void mykobj_exit(void)
{
	// 释放 kobject
	kobject_put(mykobject01);
}
module_init(mykobj_init); // 指定模块的初始化函数
module_exit(mykobj_exit); // 指定模块的退出函数
MODULE_LICENSE("GPL"); // 模块使用的许可证
MODULE_AUTHOR("topeet"); // 模块的作者