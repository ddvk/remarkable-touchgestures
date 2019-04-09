#!/bin/sh
if [ -z "$DEVICE" ]
then
    DEVICE=root@remarkable
fi

make &&
ssh $DEVICE killall touchinjector ||
scp bin/touchinjector $DEVICE:~/ && 
ssh -tt $DEVICE '/home/root/touchinjector'
