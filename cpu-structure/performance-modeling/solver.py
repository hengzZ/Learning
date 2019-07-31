import os
import sys

import numpy as np
from sklearn import linear_model
from sklearn.preprocessing import PolynomialFeatures


def sample0():
	# Transform X from [x1,x2] to [1, x1, x2, x1^2, x1x2, x2^2].
	X1 = np.arange(6).reshape(3, 2)
	X2 = PolynomialFeatures(degree=2).fit_transform(X1)
	print(X1)
	print(X2)


def sample1():
	from sklearn.pipeline import Pipeline
	model = Pipeline([('poly', PolynomialFeatures(degree=3)),
					  ('linear', linear_model.LinearRegression(fit_intercept=False))])  # 忽略截距项
	# fit to an order-3 polynomial data
	x = np.arange(5)
	y = 3 - 2*x + x**2 - x**3  # y=3-2x+x^2-x^3
	model = model.fit(x[:,np.newaxis], y)
	print(model.named_steps['linear'].coef_)


if __name__ == '__main__':
	print('sample0 —— transform X from [x1,x2] to [1, x1, x2, x1^2, x1x2, x2^2].')
	sample0()

	print('sample1 —— y = 3 - 2x + x^2 - x^3')
	sample1()

