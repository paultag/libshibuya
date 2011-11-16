#!/bin/bash

REVID=`git log | grep commit | head -n 1 | awk '{print $2}'`

echo "Running tests. Please ensure you have a curses-aware terminal."
echo ""
echo " == DBG info =="
echo "  Revno: $REVID"
echo ""
echo "Hit enter to run tests."
echo ""

read foo

./bin/run-test
if [ $? -ne 0 ]; then
	echo ""
	echo "End tests."
	echo ""
	echo "Errors detected. Hit enter to review, ctrl+c to exit."
	read foo
	./bin/review-diffs
fi
