#!/bin/bash

clang-tidy-8 `find . -type f \( -name '*.h' -o -name '*.cpp' \)` -config='' -- -std=c++11 -stdlib=libc++ -I/usr/lib/llvm-8/include/c++/v1/ -x c++
