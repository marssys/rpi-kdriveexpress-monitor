# rpi-kdriveexpress-monitor
## Hardware
- Raspberry Pi 3 Model B / Raspberry Pi 2 Model B
- Weinzierl KNX BAOS Module 838 kBerry  
![hardware](https://github.com/marssys/rpi-kdriveexpress-monitor/raw/master/images/hardware.jpg)
## Prepare Raspberry Pi 3 Model B (Raspberry Pi OS Lite)
The default usage of the UART Interface is serial console. To use the UART we must turning off the UART functioning as a serial console.
- `sudo nano /boot/cmdline.txt`
- Remove **console=serial0,115200**
The Raspberry Pi 3 does not use the correct baud rate in Raspian out of the box, so kBerry will not work. To fix this, the overlay pi3-miniuart-bt-overlay must be activated.
- `sudo sh -c "echo dtoverlay=pi3-miniuart-bt >>/boot/config.txt"`
- `sudo reboot`
## Prepare Raspberry Pi 2 Model B (Raspbian Stretch Lite)
- `sudo nano /boot/cmdline.txt`
- Remove **console=serial0,115200**
- `sudo reboot`
- Check the group of the device `ls -l /dev/ttyAMA0`
```
crw-rw---- 1 root dialout 204, 64 Aug  4 11:33 /dev/ttyAMA0
```
## Install
- Copy rpi-kdriveexpress-monitor to /home/pi (git or wget)
- Unzip (if you use wget)
- `cd rpi-kdriveexpress-monitor`
- `sudo cp lib/raspbian/libkdriveExpress.so  /usr/local/lib`
- `gcc -Iinclude -Wl,-rpath,/usr/local/lib -o rpi-kdriveexpress-monitor rpi-kdriveexpress-monitor.c -lkdriveExpress -lpthread`
- `./rpi-kdriveexpress-monitor`
## C samples
- kdrive_express_dpt.c
- kdrive_express_ip.c
- kdrive_express_ip_tunnel_enumerate.c
- kdrive_express_services.c
- kdrive_express_tiny_serial.c
- kdrive_express_tunnel_ind_addr.c
- kdrive_express_usb.c
## Build C samples
- `cd samples/c`
- `gcc -I../../include -Wl,-rpath,/usr/local/lib -o kdrive_express_ip kdrive_express_ip.c -lkdriveExpress -lpthread`
- `./kdrive_express_ip`
## First steps after install Raspbian Stretch Lite OS
- Set timezone `sudo dpkg-reconfigure tzdata`
- Change password `passwd`
- `sudo apt-get update`
- `sudo apt-get upgrade`
- Install git `sudo apt-get install git`
- Install screen `sudo apt-get install screen`
## Screen
- `sudo apt-get install screen`
- View screens `screen -ls`
- Create new screen `screen`
- Detach from screen `Ctrl+A`, then `D`
- Return to previous screen `screen -r`
- `screen -x`
- Exit from screen `exit`
