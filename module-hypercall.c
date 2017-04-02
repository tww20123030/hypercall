#include <linux/module.h>
#include <linux/kernel.h>
#include <xen/interface/xen.h>
#include <asm/xen/hypercall.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
    HYPERVISOR_hello_hypercall();
    printk("hello.\n");
    return 0;
}

static int hello_exit(void)
{
    printk("goodbye.\n");
    return 0;

}

module_init(hello_init);
module_exit(hello_exit); 

