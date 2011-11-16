#!/bin/bash

echo "Running tests. Please ensure you have a curses-aware terminal."
echo ""
./bin/run-test
if [ $? -ne 0 ]; then
	echo ""
	echo "End tests."
	echo ""
	echo "Errors detected. Hit enter to review, ctrl+c to exit."
	read foo
	./bin/review-diffs
fi
