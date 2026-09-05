#!/bin/bash

#use the -t argument to run the ctest tests

cmake --build build

if [[ "$1" == "-t" ]]; then
	echo "The test suite is being built"
	ctest --test-dir build --output-on-failure
fi
