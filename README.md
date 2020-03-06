# Project Assembler
![Assembler](misc/logo.png)
@image latex logo.png 
A Simplified MIPS Assembler for CSC3050.
## Introduction
This is simple but efficient parallel parser for MIPS assembly language. It uses the `absiel-cpp` library for efficient 
string operations and the `mimalloc` library to improve the efficiency of parallel frequent memory allocation. It supports the
whole set of MIPS 2000 assembly language, except for pseudo instructions and all instructions involved coprocessor operations.
Labels are also supported

It also has an excellent error handling system. Basically it will recover from the parsing error and generate a full error
description with exact line number and error position.

## How does it work?
The working procedure is quite simple:
- read in the file line by line
- remove comments and nonsense charactors
- assign jobs to thread parsers
- generate label map and push a job to the recover queue if a unsolved label occurs
- recover all jobs in the recover queue
- generate the final output

## Build Environment
- Linux (64bit, GNU environment)
- GNU Toolchain (GCC and binary tools, GCC >= 7, GCC 9 is recommended)
- CMake (3.12 and above)
### Preparation 
On Ubuntu, you can install the toolchains with the following code:
```bash
sudo add-apt-repository ppa:jonathonf/gcc-9.0
sudo apt-get update
sudo apt-get -y install gcc-9 cmake
```

## Build Instruction
- enter the project directory
- run `git submodule update --init --remote` if you are not using the full distribution archive 
- run build commands
```bash
mkdir build # create a new directory to build
cd build # enter the empty directory
cmake .. -DCMAKE_BUILD_TYPE=Release # generate the makefiles
make -j $(nproc) # compile the project
```
- enjoy playing with the assembler

## Important
 It has been tested, the GCC on Ubuntu 16.04 has bug on thread_local linkage.
 One possible workaround is to use clang.
```c++
 apt update
 apt install clang-6.0 libomp-dev libomp5 
 mkdir build && cd build
 env CC=clang-6.0 CXX=clang-6.0 cmake .. -DCMAKE_BUILD_TYPE=Release
 make -j $(nproc)
``` 

## How to Use
type `./assember -h` and it will tell you all:
```bash
-> ./assembler --help
easy MIPS assembler
Usage: ./assembler [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -f,--file TEXT              input path
  -o,--output TEXT            output path
```
if input or output is not given, it may use the standard ones. If so, remember to end your input with EOF (Ctrl-D).

## How to test
First, you are recommended to build the project with `-DCMAKE_BUILD_TYPE=Debug` to get more messages.

Second, just enter the following commands (in your build directory):
```bash
ctest
```

The testing contents are the followings:
- Basic register names, numbers, words, labels
- Basic I-Type lines
- Basic J-Type lines
- Basic R-Type lines
- Basic RI-Type lines
- Sample block test