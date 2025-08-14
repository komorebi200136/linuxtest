#include <linux/init.h>
#include <linux/module.h>

int param;
int data[10];
char str[10];
module_param(param, int, S_IRUGO |  S_IWUSR);

module_param_array(data, int, NULL, S_IRUGO);

module_param_string(str, str, sizeof(str), S_IRUGO);

static int __init param_init(void){
    int i=0 ;
    printk("param:%d\n", param);
    for(i=0; i<10; i++) printk("data:%d\n", data[i]);
    printk("str:%s\n", str);
    printk("param_init\n");
    return 0;
}

static void __exit param_exit(void){
    printk("param_exit\n");
}

module_init(param_init);
module_exit(param_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");
