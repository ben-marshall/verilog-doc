#!/bin/sh

echo "------------------- Setting Up Verilog-Doc Workspace -----------------"

git submodule update --remote src/verilog-parser/

mkdir -p ./build/
mkdir -p ./build/docs

cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd -

echo "----------------------- Workspace setup complete ---------------------"

