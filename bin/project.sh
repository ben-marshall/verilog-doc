#!/bin/sh

echo "------------------- Setting Up Verilog-Doc Workspace -----------------"

git submodule update --init --recursive

rm -rf ./build

mkdir -p ./build/
mkdir -p ./build/debug
mkdir -p ./build/coverage
mkdir -p ./build/docs

cd ./build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../../
cd -

cd ./build/coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DVERIDOC_COVERAGE=YES ../../
cd -

echo "----------------------- Workspace setup complete ---------------------"

