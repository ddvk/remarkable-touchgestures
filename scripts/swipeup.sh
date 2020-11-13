#!/bin/sh
DRAFT_PID="/tmp/draft"
DRAFT_KILL="/tmp/draft_kill"
if [ -f $DRAFT_PID ]; then
	if [ -f $DRAFT_KILL ]; then
		sh $DRAFT_KILL
		rm $DRAFT_KILL
	fi
    pid=$(cat $DRAFT_PID)
    rm $DRAFT_PID
    kill -9 -$pid
    #todo find a better way
    killall remarkable-shutdown
    systemctl start xochitl
else
    systemctl stop xochitl
    LD_PRELOAD=/home/root/librm2fb_server.so.1.0.0 /usr/bin/remarkable-shutdown &
    sleep 2
    LD_PRELOAD=/home/root/librm2fb_client.so.1.0.0 setsid /home/root/apps/draft &
    echo $! > $DRAFT_PID
fi
