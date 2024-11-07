#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define DRIVER_VERSION "v0.1"
#define DRIVER_AUTHOR "zhongqilong <tsilone@qq.com>"
#define DRIVER_DESC "USB Hello Driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static struct usb_device *device; // usb设备

static int picousb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    struct usb_host_interface *interface = intf->cur_altsetting;
    struct usb_endpoint_descriptor *endpoint;

    printk(KERN_INFO "Raspberrypi pico (%04X:%04X) plugged\n", id->idVendor, id->idProduct);

    printk(KERN_INFO "bInterfaceNumber i/f : %d  -- bNumEndpoints: %02X\n", interface->desc.bInterfaceNumber, interface->desc.bNumEndpoints);

    for (int i = 0; i < interface->desc.bNumEndpoints; i++)
    {
        endpoint = &interface->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n", i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n", i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
    }

    device = interface_to_usbdev(intf);

    // struct usb_bus *bus = device->bus;
    printk(KERN_INFO "USB device bus: %d %s\n", device->bus->busnum,device->bus->bus_name);

    printk(KERN_INFO "Manufacturer: %s\n", device->manufacturer);
    printk(KERN_INFO "Product: %s\n", device->product);
    printk(KERN_INFO "Chip serial number: %s\n", device->serial);

    // 如果想约束自己品牌的产品，判断产品名称，自定义自己的制造商。
    if (strcmp(device->manufacturer, "Diffi, Inc.") == 0 && strcmp(device->product, "UHF Reader") == 0){
        printk(KERN_INFO "This is Diffi UHF Reader\n");
    }

    return 0;
};

static void picousb_disconnect(struct usb_interface *intf)
{
    printk(KERN_INFO "picousb unplugged\n");
};

// 这个定义识别的设备是树莓派pico，当插入和拔除pico设备时，内核会打印出相应的信息
static struct usb_device_id picousb_table[] = {
    // raspberrypi pico vendor_id/product_id
    {USB_DEVICE(0x2e8a, 0x0003)},
    {} // Terminating entry
};

MODULE_DEVICE_TABLE(usb, picousb_table);

static struct usb_driver picousb_driver = {
    .name = "picousb",
    .id_table = picousb_table,
    .probe = picousb_probe,
    .disconnect = picousb_disconnect,
    .suspend = NULL,
    .resume = NULL,
    .pre_reset = NULL,
    .post_reset = NULL,
    .supports_autosuspend = 1,
};

static int __init picousb_init(void)
{
    int res = usb_register(&picousb_driver);
    if (res < 0)
    {
        printk(KERN_INFO "picousb driver register failed\n");
        return -1;
    }
    return 0;
};

static void __exit picousb_exit(void)
{
    usb_deregister(&picousb_driver);
};

module_init(picousb_init);
module_exit(picousb_exit);
