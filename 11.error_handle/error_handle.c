#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>


dev_t device_number_dev;
struct cdev device_number_cdev;
static struct class *class_test;
struct device *device_number_cdev_device;

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

static int __init device_number_init(void)
{
        int ret = 0;

        // 动态分配设备号
        ret = alloc_chrdev_region(&device_number_dev, 0, 1, "device_number");
        if(ret < 0) {
                printk("Failed to allocate device number\n");
                goto alloc_failed;
        }
    
        printk("Allocated device number: major = %d, minor = %d\n", 
                MAJOR(device_number_dev), MINOR(device_number_dev));

        cdev_init(&device_number_cdev, &device_number_fops);
        device_number_cdev.owner = THIS_MODULE;  //将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        ret = cdev_add(&device_number_cdev, device_number_dev, 1);
        if(ret < 0) {
                printk("Failed to add device number\n");
                goto add_failed;
        }
        
        class_test = class_create(THIS_MODULE, "device_number_class");
        if(IS_ERR(class_test)) {
                printk("Failed to create class\n");
                ret = PTR_ERR(class_test);
                goto class_failed;
        }
        device_number_cdev_device = device_create(class_test, NULL, device_number_dev, NULL, "device_number");
        if(IS_ERR(device_number_cdev_device)) {
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
        device_destroy(class_test,device_number_dev);
        class_destroy(class_test);
        cdev_del(&device_number_cdev);
        unregister_chrdev_region(device_number_dev, 1);
        printk("Device unregistered\n");
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");