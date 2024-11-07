# Linux Driver Tutorial


## Linux驱动开发环境搭建
* 查找include目录`find /usr/ -name usb.h`,fedora39在`/usr/src/kernels/$(shell uname -r)/include/`


## Reference
* [编写USB设备驱动程序](https://docs.kernel.org/driver-api/usb/writing_usb_driver.html)
* [USB Drivers in Linux](https://sysplay.github.io/books/LinuxDrivers/book/Content/Part11.html)
* [printk日志级别](https://www.kernel.org/doc/html/next/translations/zh_CN/core-api/printk-basics.html)