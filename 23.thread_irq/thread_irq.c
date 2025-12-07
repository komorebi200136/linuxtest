#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>


int key_irq;

struct workqueue_struct *test_workqueue;   //工作队列
struct delayed_work test_workqueue_work;    //工作项

irqreturn_t test_work(int irq, void *dev_id)
{
        printk("test_work\n");
        return IRQ_HANDLED;
}
irqreturn_t key_handler(int irq, void *dev_id)
{
        printk("key_handler\n");
        return IRQ_WAKE_THREAD;
}
static int __init myirq_init(void)
{
        int ret = 0;
        printk("irq_init\n");
        key_irq = gpio_to_irq(42);
        printk("key_irq = %d\n", key_irq);
        ret = request_threaded_irq(key_irq, key_handler, test_work, IRQF_TRIGGER_RISING, "key_handler",NULL);
        if (ret) {
                printk("request_irq failed\n");
                return ret;
        }
        test_workqueue = create_workqueue("myworkqueue");
        return 0;
}

static void __exit myirq_exit(void)
{
        free_irq(key_irq, NULL);
        printk("irq exit\n");
}

module_init(myirq_init);
module_exit(myirq_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");