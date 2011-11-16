#!/bin/bash

PID=`ps -ef | grep shibuya | grep -v grep | awk '{print $2}'`
/bin/kill -12 $PID
exit 0
