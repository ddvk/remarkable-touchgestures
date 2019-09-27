#!/bin/sh
#to be run on the device
wget https://github.com/ddvk/remarkable-touchgestures/releases/download/0.0.3/touch.tgz -O - | tar xvf -
systemctl enable /home/root/touch.service
systemctl start touch
