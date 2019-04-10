# Prototype

Enable page turning with the touch screen by emulatig button presses
Map actions to gestures (in progress)

## Howto compile

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
Had to reference QT for UI feedback, but the executable is now 1mb (10kb in C) and take 10x more memory (10mb), will be removed when I find the time to add direct fb operations
