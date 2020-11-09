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
    kill $pid
    #todo find a better way
    killall chess
    killall fingerterm
    killall plato
    killall edit
    killall draft
    killall remarkable-shutdown
    systemctl start xochitl
else
    systemctl stop xochitl
    /home/root/scripts/ui.sh &
    sleep 2
    export LD_PRELOAD=/home/root/librm2fb_client.so.1.0.0
    /home/root/apps/draft &
    echo $! > $DRAFT_PID
fi
