#!/bin/bash
cd app/
mkdir -p build
cd build
cmake ../src
make
valgrind --xml=yes --xml-file=XMLFile.log > Textlog.log 2>&1 --leak-check=full -s ../src/bin/runTests test
echo "finish"