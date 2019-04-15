# Touch gestures

Enables page turning with the touch screen by emulatig hardware button presses


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

## TODO
- [ ] make it configurable
- [ ] framebuffer in C/libremarkable
- [ ] draw primitives by injecting pen input (lines, rectangles)

