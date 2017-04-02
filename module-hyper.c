/*#include <linux/module.h>
#include <linux/kernel.h>
#include <xen/xen.h>
#include <asm/xen/hypercall.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
    __HYPERVISOR_hello_hypercall();
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

//may be useful in debian*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#define __HYPERVISOR_hello_hypercall 41
//You can see the all define declaration in public/xen.h
char *hypercall_stubs;
/*
#define _hypercall0(type, name) \
({\
 type __res; \
 asm volatile( \
         HYPERCALL_STR(name) \
         : "=a"(__res) \
         : \
         : "memory"); \
 (type)__res; \
})
#define HYPERCALL_STR(name) \
"mov $("__stringify(__HYPERVISOR_##name)"* 32),%%eax;"\
"add hypercall_stubs,%%eax;"\
"call *%%eax"*/
#define _hypercall0(type, name) \
({                  \
    type __res;         \
    asm volatile (      \
      "call hypercall_stubs + ("__stringify(__HYPERVISOR_##name)" * 32)"  \
      : "=a" (__res)     \
      :     \
      : "memory" );      \
    (type)__res;     \
})

static int init_hypercall(void)
{
        uint32_t eax, ebx, ecx,edx, pages, msr, i;
        char signature[13];
        /*Get current status of OS*/
        cpuid(0x40000000, &eax, &ebx, &ecx, &edx);
        *(uint32_t*)(signature + 0) = ebx;
        *(uint32_t*)(signature + 4) = ecx;
        *(uint32_t*)(signature + 8) = edx;
        signature[12] = 0;
        /*If it is run on Xen VMM */
        if(strcmp("XenVMMXenVMM", signature) || (eax < 0x40000002)) {
                printk(KERN_WARNING
                                        "Detected Xen platform device but not Xen VMM?"
                                        "(sig %s, eax %x)\n", signature, eax);
                return -EINVAL;
        }
        /* Get version of xen */
        cpuid(0x40000001, &eax, &ebx, &ecx, &edx);
        printk(KERN_INFO "Xen version %d,%d.\n", eax>>16, eax&0xffff);
        cpuid(0x40000002, &pages, &msr, &ecx, &edx);
        printk(KERN_INFO "there are %u pages\n", pages);
        hypercall_stubs = __vmalloc(pages*PAGE_SIZE, GFP_KERNEL, __pgprot(__PAGE_KERNEL &~_PAGE_NX));
        if(hypercall_stubs == NULL)
                return -ENOMEM;
        for(i=0; i<pages; i++) {
                unsigned long pfn;
                pfn = vmalloc_to_pfn((char *)hypercall_stubs + i*PAGE_SIZE);
                wrmsrl(msr, ((u64)pfn << PAGE_SHIFT) + i);
        }
        _hypercall0(int, hello_hypercall);
        printk(KERN_INFO "Hypercall area is %u pages\n", pages);
        return 0;
}

void twwclean(void)
{
}
module_init(init_hypercall);
module_exit(twwclean);
