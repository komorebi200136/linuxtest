#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

ssize_t device_number_read (struct file *file, char __user *use, size_t size, loff_t *loff) {
        printk("device_number_read\n");
        return 0;
}

ssize_t device_number_write(struct file *file, const char __user *use, size_t size, loff_t *loff) {
        printk("device_number_write\n");
        return 0;
}

int device_number_open (struct inode *inode, struct file *file) {
        printk("device_number_open\n");
        return 0;
}

int device_number_release (struct inode *inode, struct file *file) {
        printk("device_number_release\n");
        return 0;
}

struct file_operations device_number_fops= {
        .owner=THIS_MODULE,//将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        .read=device_number_read,
        .write=device_number_write,
        .open=device_number_open,
        .release=device_number_release,
};

struct miscdevice device_misc = {
       .minor = MISC_DYNAMIC_MINOR,
       .name = "device_misc",
       .fops = &device_number_fops,
};

static int __init device_number_init(void)
{
        int ret = 0;
        ret = misc_register(&device_misc);
        if (ret < 0) {
                printk("Failed to register misc device\n");
                return ret;
        }
        printk("Device number %d registered\n", device_misc.minor);
        return 0;
}

static void __exit device_number_exit(void)
{
        misc_deregister(&device_misc);
        printk("Device number %d deregistered\n", device_misc.minor);
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");