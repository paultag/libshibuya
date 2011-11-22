#!/bin/bash

for fg in {0..7}; do
	for bg in {0..7}; do
		echo -n "[3`echo $fg`;4`echo $bg`mH[0m"
	done
	echo ""
done
