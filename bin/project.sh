#!/bin/sh

echo "------------------- Setting Up Verilog-Doc Workspace -----------------"

git submodule update --remote src/verilog-parser/

mkdir -p ./build/
mkdir -p ./build/docs

cd ./build
cmake ..
cd -

echo "----------------------- Workspace setup complete ---------------------"

