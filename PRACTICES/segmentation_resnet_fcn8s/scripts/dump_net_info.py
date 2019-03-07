#!/usr/bin/python

import os
import sys

import numpy as np
import caffe


def main():
    net = caffe.Net('defect/deploy.prototxt', caffe.TEST)
    return 0


if __name__ == "__main__":
    sys.exit(main())
