#! usr/bin/bash

cd app/
mkdir -p build
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
hostname -i > temp.txt
hostname -i >> temp.txt
echo "finish build"
./asioserver
