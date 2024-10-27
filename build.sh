#!/bin/bash

# remove history compile record
rm -rf ./build/*
rm -rf ./output/*

conan profile detect --force
conan install . --output-folder=build --build=missing

# compile
cmake -G "Unix Makefiles" -S . -Bbuild -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake .
cd build/
make
make install
cd ..