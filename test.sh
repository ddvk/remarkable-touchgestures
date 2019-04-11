#!/bin/sh
if [ -z "$DEVICE" ]
then
    DEVICE=root@remarkable
fi

make  || exit 1
ssh $DEVICE killall touchinjector
arm-poky-linux-gnueabi-strip bin/touchinjector
scp bin/touchinjector $DEVICE:~/ && 
ssh -tt $DEVICE '/home/root/touchinjector'
