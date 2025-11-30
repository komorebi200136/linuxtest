#include "linux/uaccess.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/io.h>

/*LED 灯由 GPIO0_B7 控制*/


#define GPIO_SWPORT_DR_L  0xFDD60000  //数据寄存器
#define GPIO_SWPORT_DDR_L 0xFDD60008  //方向寄存器

dev_t device_number_dev;
struct cdev device_number_cdev;
static struct class *class_test;
struct device *device_number_cdev_device;
unsigned int *vir_gpio_dr;
unsigned int *vir_gpio_ddr;

ssize_t device_number_read (struct file *file, char __user *use, size_t size, loff_t *loff) {
        printk("device_number_read\n");
        return 0;
}

ssize_t device_number_write(struct file *file, const char __user *use, size_t size, loff_t *loff) {
        char buf[32] = {0};
        int ret  = 0;   
        printk("device_number_write\n");
        ret = copy_from_user(buf, use, size);
        if (ret) {
                printk("copy_from_user failed\n");
                return -EFAULT;
        }
        printk("buf = %d\n", buf[0]);
        if (buf[0] == 1) {
                *vir_gpio_dr = (*vir_gpio_dr & ~0x80008000) | 0x80008000;
                printk("led on\n");
        } else if (buf[0] == 0) {
                *vir_gpio_dr = (*vir_gpio_dr & ~0x80008000) | 0x80000000;
                printk("led off\n");
        }
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

        device_number_cdev.owner = THIS_MODULE;  //将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
        cdev_init(&device_number_cdev, &device_number_fops);
        ret = cdev_add(&device_number_cdev, device_number_dev, 1);
        if (ret < 0) {
                printk("Failed to add device number\n");
                goto add_failed;
        }
        printk("%s:%d\n",__func__,__LINE__);
        class_test = class_create(THIS_MODULE, "led_class");
        if (IS_ERR(class_test)) {
                printk("Failed to create class\n");
                ret = PTR_ERR(class_test);
                goto class_failed;
        }
        printk("%s:%d\n",__func__,__LINE__);
        device_number_cdev_device = device_create(class_test, NULL, device_number_dev, NULL, "led_device");
        if (IS_ERR(device_number_cdev_device)) {
                printk("Failed to create device\n");
                ret = PTR_ERR(device_number_cdev_device);
                goto device_failed;
        }
        printk("%s:%d\n",__func__,__LINE__);
        vir_gpio_dr = ioremap(GPIO_SWPORT_DR_L, 4); //将物理地址转化为虚拟地址
        if (IS_ERR(vir_gpio_dr)) {
                ret = PTR_ERR(vir_gpio_dr); //PTR_ERR()来返回错误代码
                printk("Failed to ioremap dr\n");
                goto err_drioremap;
        }
        vir_gpio_ddr = ioremap(GPIO_SWPORT_DDR_L, 4); //将物理地址转化为虚拟地址
        if (IS_ERR(vir_gpio_ddr)) {
                printk("Failed to ioremap ddr\n");
                ret = PTR_ERR(vir_gpio_ddr); //PTR_ERR()来返回错误代码
                goto err_ddrioremap;
        }
        *vir_gpio_ddr = (*vir_gpio_ddr & ~0x80008000) | 0x80008000;
        return ret;  //返回 0 表示初始化成功，如果不这样做，后面的错误处理会继续运行，将设备都卸载了
err_ddrioremap:
        iounmap(vir_gpio_dr);
err_drioremap:
       device_destroy(class_test,device_number_dev);
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
    
    // 5. 还需要添加ioremap的释放
    if (vir_gpio_dr) {
        iounmap(vir_gpio_dr);
        vir_gpio_dr = NULL;
    }
    if (vir_gpio_ddr) {
        iounmap(vir_gpio_ddr);
        vir_gpio_ddr = NULL;
    }
    
    printk("Device number unregistered\n");
}

module_init(device_number_init);
module_exit(device_number_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");

