# Linux Driver Tutorial

* 进入响应的目录，执行`make`命令编译驱动程序，执行`make clean`命令清除编译结果。
* 安装和卸载驱动：`sudo insmod hello.ko`和`sudo rmmod hello`
* 查看日志：`sudo dmesg | tail -n20`


## Linux驱动开发环境搭建
* 查找include目录`find /usr/ -name usb.h`,fedora39在`/usr/src/kernels/$(shell uname -r)/include/`
* 安装头文件依赖：`sudo dnf install kernel-devel kernel-headers`


## Reference
* [编写USB设备驱动程序](https://docs.kernel.org/driver-api/usb/writing_usb_driver.html)
* [USB Drivers in Linux](https://sysplay.github.io/books/LinuxDrivers/book/Content/Part11.html)
* [printk日志级别](https://www.kernel.org/doc/html/next/translations/zh_CN/core-api/printk-basics.html)