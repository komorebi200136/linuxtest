#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>


dev_t device_number_dev;
struct cdev device_number_cdev;
struct file_operations device_number_fops= {
        .owner=THIS_MODULE //将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
};;

static int __init device_number_init(void)
{
        int ret = 0;

        ret = register_chrdev_region(device_number_dev, 1, "device_number");
        if(ret < 0) {
                printk("Failed to register device number\n");
                return ret;
        }
        cdev_init(&device_number_cdev, &device_number_fops);
        device_number_cdev.owner = THIS_MODULE;  //将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        ret = cdev_add(&device_number_cdev, device_number_dev, 1);
        if(ret < 0) {
                printk("Failed to add device number\n");
                return ret;
        }
        return 0;

}

static void __exit device_number_exit(void)
{
        cdev_del(&device_number_cdev);
        unregister_chrdev_region(device_number_dev, 1);
        printk("Device number %d unregistered\n", device_number_dev);
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");