#obj-m += module-hypercall.o
obj-m += module-printk.o
all:
	  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
