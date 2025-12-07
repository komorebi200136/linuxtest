#include "linux/uaccess.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/semaphore.h>

dev_t device_number_dev;
struct cdev device_number_cdev;
static struct class *class_test;
struct device *device_number_cdev_device;

struct semaphore sem_test;

ssize_t device_number_read (struct file *file, char __user *use, size_t size, loff_t *loff) {
        printk("device_number_read\n");
        return 0;
}

ssize_t device_number_write(struct file *file, const char __user *use, size_t size, loff_t *loff) {
        printk("device_number_write\n");
        return 0;
}

int device_number_open (struct inode *inode, struct file *file) {
        down(&sem_test);
        printk("device_number_open\n");
        return 0;
}

int device_number_release (struct inode *inode, struct file *file) {
        printk("device_number_release\n");
        up(&sem_test);
        return 0;
}

struct file_operations device_number_fops= {
        .owner=THIS_MODULE,//将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        .read=device_number_read,
        .write=device_number_write,
        .open=device_number_open,
        .release=device_number_release,
};

static int __init device_number_init(void)
{
        int ret = 0;

        sema_init(&sem_test, 1);

        // 动态分配设备号
        ret = alloc_chrdev_region(&device_number_dev, 0, 1, "device_number");
        if(ret < 0) {
                printk("Failed to allocate device number\n");
                goto alloc_failed;
        }
    
        printk("Allocated device number: major = %d, minor = %d\n", 
                MAJOR(device_number_dev), MINOR(device_number_dev));

        device_number_cdev.owner = THIS_MODULE;  //将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        cdev_init(&device_number_cdev, &device_number_fops);
        ret = cdev_add(&device_number_cdev, device_number_dev, 1);
        if (ret < 0) {
                printk("Failed to add device number\n");
                goto add_failed;
        }
        class_test = class_create(THIS_MODULE, "led_class");
        if (IS_ERR(class_test)) {
                printk("Failed to create class\n");
                ret = PTR_ERR(class_test);
                goto class_failed;
        }
        device_number_cdev_device = device_create(class_test, NULL, device_number_dev, NULL, "led_device");
        if (IS_ERR(device_number_cdev_device)) {
                printk("Failed to create device\n");
                ret = PTR_ERR(device_number_cdev_device);
                goto device_failed;
        }
        return ret;  //返回 0 表示初始化成功，如果不这样做，后面的错误处理会继续运行，将设备都卸载了
device_failed:
        class_destroy(class_test);
class_failed:
        cdev_del(&device_number_cdev);
add_failed:
        unregister_chrdev_region(device_number_dev, 1);
alloc_failed:
        return ret;

}

static void __exit device_number_exit(void)
{
    // 1. 首先删除设备（最后创建的）
    device_destroy(class_test, device_number_dev);
    
    // 2. 删除类
    class_destroy(class_test);
    
    // 3. 删除字符设备
    cdev_del(&device_number_cdev);
    
    // 4. 最后注销设备号（最先申请的）
    unregister_chrdev_region(device_number_dev, 1);

    printk("Device number unregistered\n");
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");

