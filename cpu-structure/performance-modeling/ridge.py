# coding: utf-8
import os
import sys

import numpy as np
from sklearn.preprocessing import PolynomialFeatures
from sklearn import linear_model
import csv


def feature_transformation(features, degree):
	poly = PolynomialFeatures(degree=degree, include_bias=False)
	return poly.fit_transform(features)


def export(X, Y, coef, intercept):
    with open('report.csv', 'w', newline='') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(X)
        csv_writer.writerow(Y)
        csv_writer.writerow(coef)
        csv_writer.writerow(intercept)


#X = np.arange(6).reshape(2, 3)
#Y = np.arange(2).reshape(2, 1)
X = np.array(np.mat(
    """
    1 2 3;
    4 5 6
    """))
Y = np.array(np.mat(
    """
    1;
    2
    """))


if __name__ == '__main__':
	new_features = feature_transformation(X, 2)

	reg = linear_model.Ridge(alpha=.5)	
	reg.fit(new_features, Y)

	export(new_features, Y, reg.coef_, reg.intercept_)

	print(new_features)
	print(reg.coef_)
	print(reg.intercept_)

