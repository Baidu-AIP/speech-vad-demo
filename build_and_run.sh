#!/usr/bin/env bash
set -uex
check_version(){
    CMD=$1
    OPT=$2
    VERSION=$3

    #$CMD $OPT
    RES=$($CMD $OPT |head -1)
    echo $RES
   
}

check_version "gcc" "--version" "4.8.2"
check_version "cmake" "--version" "2.8"
sleep 2;
echo ""
mkdir -p build && \
mkdir -p output_pcm && \
rm -rf  output_pcm/* build/* && \
cd build && \
cmake .. && \
make -j4 && \
cp vad-demo .. && cd .. && \
echo "build success wait 3s to run" && \
sleep 3 && \
./vad-demo
 
