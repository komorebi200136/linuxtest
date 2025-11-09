#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

int device_number_major = 0;
int device_number_minor = 0;
module_param(device_number_major,int,S_IRUGO);
module_param(device_number_minor,int,S_IRUGO);

dev_t device_number_dev;

static int __init device_number_init(void)
{
    int ret = 0;
    if(!device_number_major) {
        printk("Invalid device number\n");
        ret = alloc_chrdev_region(&device_number_dev, 0, 1, "device_number");
        if(ret < 0) {
            printk("Failed to allocate device number\n");
            return ret;
        }
        device_number_major = MAJOR(device_number_dev);
        device_number_minor = MINOR(device_number_dev);
        printk("Allocated device number %d\n", device_number_major);
        printk("Allocated device number %d\n", device_number_minor);
    } else {
        device_number_dev = MKDEV(device_number_major, device_number_minor);
        printk("Using device number %d\n", device_number_major);
        ret = register_chrdev_region(device_number_dev, 1, "device_number");
        if(ret < 0) {
            printk("Failed to register device number\n");
            return ret;
        }
    }
    
    return 0;

}

static void __exit device_number_exit(void)
{
    unregister_chrdev_region(device_number_dev, 1);
    printk("Device number %d unregistered\n", device_number_major);
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");