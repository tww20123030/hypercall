#include <linux/module.h>
#include <linux/kernel.h>
#include <xen/interface/xen.h>
#include <asm/xen/hypercall.h>

//MODULE_LICENSE("Dual BSD/GPL");

static int printk_enable(void)
{
    HYPERVISOR_printk_op();
    printk("printk_op.\n");
    return 0;
}

static void printk_disable(void)
{
    HYPERVISOR_printk_close();
    printk("printk_close\n");
    return;

}

MODULE_LICENSE("GPL");
module_init(printk_enable);
module_exit(printk_disable); 

