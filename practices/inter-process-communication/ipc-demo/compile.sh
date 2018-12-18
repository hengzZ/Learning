#!/bin/bash

gcc shm.c -o shm
gcc pipe.c -o pipe
gcc mman.c -o mman
gcc -pthread sem.c -o sem
