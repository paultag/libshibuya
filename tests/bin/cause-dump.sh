#!/bin/bash

PID=`ps -ef | grep shibuya | grep -v grep | awk '{print $2}'`
if [ "x$PID" == "x" ]; then exit 0; fi

/bin/kill -10 $PID
exit 0
