#!/bin/bash 

# Required Packages
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev 
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev 

# Working Directory
mkdir -p opencv-install
cd opencv-install 

# Getting the Cutting-edge OpenCV from the Git Repository 
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

# Building OpenCV from Source Using CMake
mkdir -p build 
cd build

cmake -D CMAKE_BUILD_TYPE=Release  \
  -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules/ \
  -D PYTHON2_EXECUTABLE=/usr/bin/python \
  -D PYTHON_INCLUDE_DIR=/usr/include/python2.7/ \
  -D PYTHON_INCLUDE_DIR2=/usr/include/x86_64-linux-gnu/python2.7/ \
  -D PYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so \
  -D PYTHON2_NUMPY_INCLUDE_DIRS=/usr/lib/python2.7/dist-packages/numpy/core/include/ \
  -D CMAKE_INSTALL_PREFIX=/usr/local ../opencv

# Make
make -j7
# install Libraries
sudo make install
