#!/bin/bash

PID=`ps -ef | grep shibuya | grep -v grep | awk '{print $2}'`
/bin/kill -10 $PID
exit 0
