# rpi-kdriveexpress-monitor
## Prepare Raspberry Pi 2 (Raspbian Stretch Lite)
'sudo nano /boot/config.txt'
- Remove **console=serial0,115200**
- 'sudo reboot'
- Check the group of the device 'ls -l /dev/ttyAMA0'
'''
crw-rw---- 1 root dialout 204, 64 Aug  4 11:33 /dev/ttyAMA0
'''
## Install
- Copy rpi-kdriveexpress-monitor to /home/pi (git or wget)
- Unzip (if you use wget)
- 'cd rpi-kdriveexpress-monitor'
- 'sudo cp lib/raspbian/libkdriveExpress.so  /usr/local/lib'
- 'gcc -Iinclude -Wl,-rpath,/usr/local/lib -o rpi-kdriveexpress-monitor rpi-kdriveexpress-monitor.c -lkdriveExpress -lpthread'
- './rpi-kdriveexpress-monitor'
