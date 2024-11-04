#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define DRIVER_VERSION "v0.1"
#define DRIVER_AUTHOR "zhongqilong <tsilone@qq.com>"
#define DRIVER_DESC "USB Hello Driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static int hellousb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    // printk(KERN_INFO "Hello, USB World!\n");
    printk(KERN_INFO "hellousb drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
};

static void hellousb_disconnect(struct usb_interface *intf){
    printk(KERN_INFO "hellousb drive unplugged\n");
};

// 这个定义识别的设备是树莓派pico，当插入和拔除pico设备时，内核会打印出相应的信息
static struct usb_device_id hellousb_table[] = {
    // raspberrypi pico vendor_id/product_id
    { USB_DEVICE(0x2e8a, 0x0003) },
    { } // Terminating entry
};

MODULE_DEVICE_TABLE(usb, hellousb_table);

static struct usb_driver hellousb_driver = {
    .name = "hellousb",
    .id_table = hellousb_table,
    .probe = hellousb_probe,
    .disconnect = hellousb_disconnect,
    .suspend = NULL,
    .resume = NULL,
    .pre_reset = NULL,
    .post_reset = NULL,
    .supports_autosuspend = 1,
};

static int __init hellousb_init(void){
    int res = usb_register(&hellousb_driver);
    if(res < 0){
        printk(KERN_INFO "hellousb driver register failed\n");
        return -1;
    }
    return 0;
};

static void __exit hellousb_exit(void){
    usb_deregister(&hellousb_driver);
};

module_init(hellousb_init);
module_exit(hellousb_exit);

