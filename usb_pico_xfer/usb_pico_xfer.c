#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

// #define MIN(a, b) ((a) <= (b) ? (a) : (b))
// #define BULK_EP_OUT 0x01
// #define BULK_EP_IN 0x82
#define BULK_EP_OUT 0x03
#define BULK_EP_IN 0x84
#define MAX_PACKET_SIZE 512

#define DRIVER_VERSION "v0.1"
#define DRIVER_AUTHOR "zhongqilong <tsilone@qq.com>"
#define DRIVER_DESC "Raspberry pi Pico USB transfer driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static struct usb_device *device;               // usb设备
static struct usb_class_driver class;           // usb类驱动
static unsigned char bulk_buf[MAX_PACKET_SIZE]; // 数据缓冲区

static int picousb_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "picousb_open\n");
    return 0;
};

static int picousb_close(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "picousb_close\n");
    return 0;
};

static ssize_t picousb_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    // printk(KERN_INFO "picousb_read %d\n", count);

    int read_count;
    int retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN), bulk_buf, MAX_PACKET_SIZE, &read_count, 5000);
    if (retval > 0)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, MIN(count, read_count)))
    {
        return -EFAULT;
    }

    return MIN(count, read_count);
};

static ssize_t picousb_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
    // printk(KERN_INFO "picousb_write %d\n", cnt);

    int retval;
    int wrote_cnt = MIN(cnt, MAX_PACKET_SIZE);

    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PACKET_SIZE)))
    {
        return -EFAULT;
    }

    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
                          bulk_buf, MIN(cnt, MAX_PACKET_SIZE), &wrote_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }

    return wrote_cnt;
};

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = picousb_open,
    .release = picousb_close,
    .read = picousb_read,
    .write = picousb_write,
};

// probe回调函数
static int
picousb_probe(struct usb_interface * intf, const struct usb_device_id *id)
{

    device = interface_to_usbdev(intf);

    // 动态创建：/dev/pico0
    class.name = "usb/pico%d";
    class.fops = &fops;

    int res = usb_register_dev(intf, &class);

    if (res < 0)
    {
        printk(KERN_ERR "Not able to register device\n");
    }
    else
    {
        printk(KERN_INFO "picousb obtained: %d\n", intf->minor);
    }

    return res;
};

static void picousb_disconnect(struct usb_interface *intf)
{
    // printk(KERN_INFO "picousb unplugged\n");
    usb_deregister_dev(intf, &class);
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
    if (res)
    {
        printk(KERN_ERR "picousb driver register failed\n");
    }
    return res;
};

static void __exit picousb_exit(void)
{
    usb_deregister(&picousb_driver);
};

module_init(picousb_init);
module_exit(picousb_exit);

// 说明：确保尚未加载 usb 存储驱动程序。否则，看不到读写方法执行并打印日志，因为系统内置驱动接管了。（Attached scsi generic sg2 type 0）
