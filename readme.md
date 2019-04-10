# Prototype

Enable page turning with the touch screen by emulatig button presses
Map actions to gestures (in progress)


## Features

touch with 2 finger around 10cm apart to enable/disable

swipe up to show current time (by default UTC, timezone info needs to be set on the device)

*when enabled*:

swipe left/right to turn pages
touch left/right 1/3 of the screen to turn pages
swipe down to enter sleep mode


## Compilation

install the dev tools from remarkable

```bash
source /opt/poky/2.1.3/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
qmake
make
```


## Installation

copy touch.service to /etc/systemd/system/

copy touchinjector to /home/root/

SSH to the device

```
systemctl start touch

systemctl enable touch (to enable on boot).
```


**Note**

Had to reference QT for UI feedback, but the executable is now 1mb (10kb in C) and takes 10x more memory (10mb vs 1mb), will be removed when I find the time to add direct fb operations
