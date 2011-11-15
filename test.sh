#!/bin/bash

#ps -ef

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
sleep 1
echo -n "[40G" # Center the cursor on the current line
sleep 1
echo -n "[K"
sleep 1
echo -n "[1K"
sleep 1

echo -n "[H"
