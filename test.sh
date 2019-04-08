#!/bin/sh
DEVICE=rm
make &&
ssh root@$DEVICE killall touchinjector ||
scp bin/touchinjector root@$DEVICE:~/ && 
ssh -tt root@$DEVICE '/home/root/touchinjector'
