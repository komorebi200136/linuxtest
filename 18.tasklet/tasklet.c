#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

int key_irq;
struct tasklet_struct mytasklet;

void tasklet_test(unsigned long data)
{
    printk("tasklet_func:%ld\n",data);
}

irqreturn_t key_handler(int irq, void *dev_id)
{
    printk("key_handler");
    tasklet_schedule(&mytasklet);
    return IRQ_HANDLED;
}
static int __init myirq_init(void)
{
    int ret = 0;
    printk("irq_init");
    key_irq = gpio_to_irq(42);
    printk("key_irq = %d", key_irq);
    ret = request_irq(key_irq,key_handler,IRQF_TRIGGER_RISING,"key_handler",NULL);
    if (ret) {
        printk("request_irq failed");
        return ret;
    }
    tasklet_init(&mytasklet, tasklet_test, 1);  //初始后默认为使能状态
    return 0;
}

static void __exit myirq_exit(void)
{
    free_irq(key_irq, NULL);
    tasklet_kill(&mytasklet); 
    printk("irq exit");
}

module_init(myirq_init);
module_exit(myirq_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");