# coding: utf-8
import os
import sys

import numpy as np
from sklearn.preprocessing import PolynomialFeatures
from sklearn import linear_model
import csv


def feature_transformation(features, degree):
    poly = PolynomialFeatures(degree=degree, include_bias=False)
    new_features = poly.fit_transform(features)
    powers = poly.powers_
    return powers, new_features


def data_preprocess(features, degree_list=[]):
    maxdeg = 0
    for idx in range(features.shape[1]):  # 列
        maxdeg = maxdeg + degree_list[idx]
    powers, trans = feature_transformation(features, maxdeg)
    ret = None
    pret = []
    for i, power in enumerate(powers):
        ppass = False
        # 判断是否符合degree约束
        for j, p in enumerate(power):
            if p > degree_list[j]:
                ppass = True
                break
        if ppass is True:
            continue
        # 提取当前列
        pret.append(power)
        col = trans[:, i][..., np.newaxis]
        # 聚合
        if ret is None:
            ret = col
        else:
            ret = np.hstack((ret, col))
    return pret, ret


X = np.array(np.mat(
    """
6	1.594456999;
6	2.090573917;
6	2.385456208;
12	1.597523713;
12	2.094428599;
12	2.39186971;
24	1.598727078;
24	2.096559978;
24	2.395586949
    """))
Y = np.array(np.mat(
    """
10654;
13931;
15308;
26012;
29140;
32484;
47334;
54655;
58913
    """))

if __name__ == '__main__':
    powers, new_features = data_preprocess(X, degree_list=[1, 1])

    reg = linear_model.Ridge(alpha=.5)
    #reg = linear_model.LinearRegression()
    reg.fit(new_features, Y)

    print('powers:\n', powers)
    print('data:\n', new_features)
    print('coef:\n', reg.coef_)
    print('intercept:\n', reg.intercept_)
