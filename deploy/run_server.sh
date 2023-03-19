#! usr/bin/bash

mkdir -p /app/build && rm -rf /app/build

cmake -DCMAKE_BUILD_TYPE=Release -S /app -B /app/build
cmake --build /app/build -j4
echo "finish build"

/app/build/main
