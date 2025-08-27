#!/bin/bash

echo "Running Logger Tests..."
echo

cd ../build/bin

if [ -f "./LoggerTest" ]; then
    echo "Found LoggerTest, running tests..."
    ./LoggerTest
    if [ $? -eq 0 ]; then
        echo
        echo "All tests passed! ✅"
    else
        echo
        echo "Some tests failed! ❌"
    fi
else
    echo "LoggerTest not found. Please build the project first."
    echo "Run: cmake --build .. from the build directory"
fi

echo

