#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>


int key_irq;

struct workqueue_struct *test_workqueue;   //工作队列
struct delayed_work test_workqueue_work;    //工作项

void myworkqueue_func(struct work_struct *work)
{
        printk("mydelaaytest_workqueue\n");
}

irqreturn_t key_handler(int irq, void *dev_id)
{
        printk("key_handler\n");
        queue_delayed_work(test_workqueue, &test_workqueue_work, msecs_to_jiffies(3000));
        return IRQ_HANDLED;
}
static int __init myirq_init(void)
{
        int ret = 0;
        printk("irq_init\n");
        key_irq = gpio_to_irq(42);
        printk("key_irq = %d\n", key_irq);
        ret = request_irq(key_irq,key_handler,IRQF_TRIGGER_RISING,"key_handler",NULL);
        if (ret) {
                printk("request_irq failed\n");
                return ret;
        }
        test_workqueue = create_workqueue("myworkqueue");
        INIT_DELAYED_WORK(&test_workqueue_work, myworkqueue_func); // 初始化工作项
        return 0;
}

static void __exit myirq_exit(void)
{
        free_irq(key_irq, NULL);
        cancel_delayed_work_sync(&test_workqueue_work);
        flush_workqueue(test_workqueue);
        destroy_workqueue(test_workqueue);
        printk("irq exit\n");
}

module_init(myirq_init);
module_exit(myirq_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("csh");