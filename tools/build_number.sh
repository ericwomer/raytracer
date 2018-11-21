#!/bin/bash

build_number=`cat build_number.txt`
let build_number++
echo "$build_number" | tee build_number.txt

