# Touch gestures

Enables page turning with the touch screen by emulatig hardware button presses


## Features

**touch with 2 fingers**, around 10-11cm apart to toggle between SwipeOnly, SwipeAndTouch or Disabled

**swipe down** to show the current time (by default UTC, timezone info needs to be set on the device, overwrite /etc/localtime)

*when SwipeOnly*:

**swipe left/right** to turn pages

**swipe up**  to execute a hardcoded script `~/scripts/swipeup.sh`

*when SwipeAndTouch*:

**touch left/right** 1/3 of the screen to turn pages (except upper and left 100 px where the toolbar usually is)


## Compilation

install the dev tools from remarkable

```bash
source /usr/local/oecore-x86_64/environment-setup-cortexa9hf-neon-oe-linux-gnueabi
qmake
make
```


## Installation

### Easy
ssh to the device

paste the following code
```
wget https://raw.githubusercontent.com/ddvk/remarkable-touchgestures/master/install.sh -O- |sh
```

### Manual

copy touch.service to /etc/systemd/system/

copy touchinjector to /home/root/

SSH to the device

```
systemctl start touch

systemctl enable touch (to enable on boot).
```

## TODO
- [ ] fix/setup the CI
- [ ] add unit tests
- [ ] make it configurable
- [ ] framebuffer in C/libremarkable

