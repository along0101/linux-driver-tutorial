#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define DRIVER_VERSION "v0.1"
#define DRIBER_AUTHOR "zhongqilong <tsilone@qq.com>"
#define DRIVER_DESC "USB Hello Driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static int hellousb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    // printk(KERN_INFO "Hello, USB World!\n");
    printk(KERN_INFO "hellousb drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
}

static struct usb_driver hellousb_driver = {
    .name = "hellousb",
    .id_table = hellousb_table,
    .probe = hellousb_probe,
    .disconnect = NULL,
    .suspend = NULL,
    .resume = NULL,
    .pre_reset = NULL,
    .post_reset = NULL,
    .supports_autosuspend = 1,
};
