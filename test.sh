#!/bin/bash

default() {
	clear
	echo -n "[13;40H" # center
}

echo -n "[H"
clear
ls -lah /

sleep 3

echo -n "[13;40H"
sleep 1
echo -n "[3M"
sleep 1
echo -n "[1M"
sleep 1
echo -n "[1M"

sleep 1
clear

default

echo "[H"

echo -n "[10d10th Line"
echo -n "[5d5th Line"
echo -n "[1dFirst Line"

sleep 1
default

U="[A"
D="[B"
F="[C"
B="[D"

echo -n "U$U U$U U$U"
sleep 1

default
echo -n "D$D D$D D$D"
sleep 1

default
echo -n "F$F F$F F$F"
sleep 1

default
echo -n "B$B B$B B$B"
sleep 1

ps -ef

echo -n "[13;40H" # Center the cursor
# Test ED
sleep 1
echo -n "[J"
sleep 1
echo -n "[1J"
sleep 1

clear
echo -n "[H"
echo -n "This is a rather long line. Im using it to test removing bits of a str"
echo -n "[40G" # Center the cursor on the current line
sleep 1
echo -n "[K"
sleep 1
echo -n "[1K"
sleep 1

echo -n "[H"
