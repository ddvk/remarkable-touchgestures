#!/bin/sh
if [ -z "$DEVICE" ]
then
    DEVICE=rm
fi

make  || exit 1
ssh $DEVICE killall touchinjector
arm-oe-linux-gnueabi-strip bin/touchinjector
scp bin/touchinjector $DEVICE:~/ && 
ssh -tt $DEVICE '/home/root/touchinjector'
