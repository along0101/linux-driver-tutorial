#include<linux/module.h>
#include<linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhongqilong");
MODULE_DESCRIPTION("A simple hello world module");

static int __init hello_init(void){
    // printk(KERN_INFO "Hello, world!\n");
    printk("Hello, world!\n");
    return 0;
}

static void __exit hello_exit(void){
    // printk(KERN_INFO "Goodbye, world!\n");
    printk("Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

// usage:
//      make
//      sudo insmod hello.ko
//      sudo dmesg | tail
//      sudo rmmod hello