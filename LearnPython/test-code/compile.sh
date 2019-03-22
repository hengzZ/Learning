#!/bin/bash
g++ -Wall -O3 -std=c++11 -shared -fPIC -I/usr/include/python2.7 -lboost_python -lopencv_core -lopencv_imgproc -lopencv_highgui -fopenmp input_loader.cpp -o input_loader.so
