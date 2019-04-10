# Proof of Concept

Enable page turning with the touch screen by emulatig button presses


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
