#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int __init lkp_init(void)
{
    //printk(KERN_ALERT "Hello World!/n");
    printk(KERN_ERR "Hello World!\n");
    printk(KERN_ERR "Hello World a!\n");
    printk(KERN_ERR "Hello World b!\n");
    printk(KERN_ERR "Hello World c!\n");
    printk(KERN_ERR "Hello World d!\n");
    //for (int i = 0; i < 100; i++) {
    //    printk(KERN_ERR "Hello World %d!/n", i);
        //sleep (1000);
    //}
    return 0;
}
static void __exit lkp_cleanup(void)
{
    printk(KERN_ERR "Hello World z!\n");
}

module_init(lkp_init);
module_exit(lkp_cleanup);
MODULE_AUTHOR("binli");
MODULE_DESCRIPTION("hello");
