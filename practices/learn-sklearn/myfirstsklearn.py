#!/usr/bin/python 

import os
import sys

import numpy as np
import scipy
from sklearn import datasets


def main(argv):
    iris = datasets.load_iris()
    digits = datasets.load_digits()

    print digits.data
    print digits.target
    print digits.images[0]


if __name__=="__main__":
    main(sys.argv)
