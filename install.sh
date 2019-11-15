#!/bin/sh
#to be run on the device
wget https://github.com/ddvk/remarkable-touchgestures/releases/download/0.0.4/touch.tgz -O - | tar xvf -
cp /home/root/touch.service /etc/systemd/system
systemctl enable touch
systemctl start touch
