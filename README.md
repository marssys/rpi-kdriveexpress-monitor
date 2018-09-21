# rpi-kdriveexpress-monitor
## Hardware
![hardware](https://github.com/marssys/rpi-kdriveexpress-monitor/raw/master/images/hardware.jpg)
## Prepare Raspberry Pi 2 (Raspbian Stretch Lite)
- `sudo nano /boot/config.txt`
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
