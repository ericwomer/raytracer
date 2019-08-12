#!/bin/bash

clang-format -i -style=file -verbose `find . -type f \( -name '*.h' -o -name '*.cpp' \)` "$@"

