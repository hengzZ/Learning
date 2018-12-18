#!/bin/bash
export MKLDNN_ROOT=mkl-dnn
export LD_LIBRARY_PATH=${MKLDNN_ROOT}/build/src:${MKLDNN_ROOT}/external/mklml_lnx_2019.0.1.20180928/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

g++ -Wall -std=c++11 main.cpp -I${MKLDNN_ROOT}/include -L${MKLDNN_ROOT}/build/src -lmkldnn -o test
echo "done"
