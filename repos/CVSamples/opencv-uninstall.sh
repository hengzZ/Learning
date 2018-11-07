#!/bin/bash 

sudo apt-get autoremove -y libopencv-dev
sudo find / -name "*opencv*" -exec rm -i {} \;
