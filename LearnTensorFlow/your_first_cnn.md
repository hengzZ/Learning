# Your First TF CNN
A sample/quick start to create your first tensorflow cnn.


## Tensorflow - your first cnn
#### 1. prerequisite knowledge
在使用 tensorflow 时，会发现 tf.nn，tf.layers， tf.contrib 模块的很多功能是重复的，尤其是卷积操作. <br>
比如：<br>
tf.nn.conv2d 与 tf.layers.conv2d 在功能上事相同的.

下面是对三个模块的简述：<br>
1. tf.nn： 提供神经网络相关操作的支持，包括卷积操作 (conv)、池化 (pooling)、归一化 (norm)、 loss 等等。可以认为: tf.nn 提供了卷积神经网络最底层的支持。
1. tf.layers：顾名思义，在 tf.nn 的基础上进一步封装，使得对神经网络的建立变得更加简单。
1. tf.contrib：提供将计算图中的网络层、正则化等构建计算图的高级操作。但是 tf.contrib 包含不稳定和实验代码，有可能以后 API 会改变。

#### 2. your firt network
```python
# coding: utf8
import os
import sys

import numpy as np
import tensorflow as tf


def Convolution(name, bottom, out_channels, kernel_h, kernel_w, stride_h, stride_w, pad_type, with_type):
	"""
	pad_type: "VALID" or "SAME"
	with_type: "None" or "relu"
	# tensor: [batch, height, width, channel]
	# stride: [1, stride_h, stride_w, 1]
	# filter: [height, width, in_channels, out_channels]
	# tf.nn.conv2d(input, filter, strides, padding, use_cudnn_on_gpu=None, data_format='NHWC', name=None)
	"""
	with tf.variable_scope(name):
		in_channels = bottom.shape[3].value
		filter = tf.Variable(tf.random_normal([kernel_h, kernel_w, in_channels, out_channels]))
		conv = tf.nn.conv2d(input=bottom, filter=filter, strides=[1, stride_h, stride_w, 1], padding=pad_type)
		conv_biases = tf.Variable(tf.constant(0.0,shape=[out_channels]))
		bias = tf.nn.bias_add(conv, conv_biases)
		if ("relu" == with_type.lower()):
			relu = tf.nn.relu(bias)
			return relu
		else:
			return bias


def Pooling(name, bottom, kernel_h, kernel_w, stride_h, stride_w, pool_type, pad_type):
	"""
	pad_type: "VALID" or "SAME"
	pool_type: "AVG" or "MAX"
	# tf.nn.pool(input, window_shape, pooling_type, padding, dilation_rate=None, strides=None, name=None, data_format=None)
	"""
	with tf.variable_scope(name):
		pool = tf.nn.pool(bottom, window_shape=[kernel_h,kernel_w], strides=[stride_h, stride_w],
				pooling_type=pool_type, padding=pad_type)
		return pool


def BatchNorm(name, bottom, epsilon, with_type, scale_out=1.0):
	"""
	variance_epsilon: A small float number to avoid dividing by 0.
	# tf.nn.batch_normalization(x, mean, variance, offset, scale, variance_epsilon, name=None)
	"""
	with tf.variable_scope(name):
		mean = tf.Variable(tf.random_normal([bottom.shape[3].value]))
		variance = tf.Variable(tf.random_normal([bottom.shape[3].value]))
		scale = tf.Variable(tf.random_normal([bottom.shape[3].value]))
		offset = tf.Variable(tf.random_normal([bottom.shape[3].value]))
		batch_norm = tf.nn.batch_normalization(bottom, mean=mean, variance=variance, offset=offset, scale=scale, variance_epsilon=epsilon)
		if ("relu" == with_type.lower()):
			relu = tf.nn.relu(batch_norm)
			return relu
		else:
			return batch_norm


def Sum(name, bottom1, bottom2, scale_in1, scale_in2, with_type):
	"""

	"""
	with tf.variable_scope(name):
		if ("relu" == with_type.lower()):
			relu = tf.nn.relu(bottom1+bottom2)
			return relu
		else:
			return bottom1 + bottom2


def make_fake_input(batch_size, input_height, input_width, input_channel):
	im = np.zeros((input_height,input_width,input_channel), np.uint8)
	im[:,:,:] = 1
	images = np.zeros((batch_size, input_height, input_width, input_channel), dtype=np.float32)
	for i in xrange(batch_size):
		images[i, 0:im.shape[0], 0:im.shape[1], :] = im
		#channel_swap = (0, 3, 1, 2)  # caffe
		#images = np.transpose(images, channel_swap)
		#cv2.imwrite("test.jpg", im)
	return images


def create_model(images):
	#
	conv1 = Convolution("layer1_layer1_conv_Conv2D", images, 64, 7, 7, 2, 2, "SAME", "relu")
	max_pool = Pooling("layer2_MaxPool2D_MaxPool", conv1, 3, 3, 2, 2, "MAX", "SAME")
	res1_1 = BatchNorm("layer2_block0_common_bn_relu_FusedBatchNorm", max_pool, 0.0010000000475, "relu")
	# cfg[1]
	res2_1b2 = Convolution("layer2_block0_sub1_sub1_conv_Conv2D", res1_1, 64, 1, 1, 1, 1, "SAME", "relu")
	res2_1b2 = Convolution("layer2_block0_sub2_sub2_conv_Conv2D", res2_1b2, 64, 3, 3, 1, 1, "SAME", "relu")
	res2_1b2 = Convolution("layer2_block0_sub3_sub3_conv_Conv2D", res2_1b2, 256, 1, 1, 1, 1, "SAME", "none")
	res2_1b1 = Convolution("layer2_block0_shortcut_sub_sc_Conv2D", res1_1, 256, 1, 1, 1, 1, "SAME", "none")
	res2_1_sum = Sum("layer2_block0_shortcut_add", res2_1b1, res2_1b2, 1.0, 1.0, "none")
	res2_1 = BatchNorm("layer2_block1_residual_bn_relu_FusedBatchNorm", res2_1_sum, 0.0010000000475, "relu")
	#
	res2_2b2 = Convolution("layer2_block1_sub1_sub1_conv_Conv2D", res2_1, 64, 1, 1, 1, 1, "SAME", "relu")
	res2_2b2 = Convolution("layer2_block1_sub2_sub2_conv_Conv2D", res2_2b2, 64, 3, 3, 1, 1, "SAME", "relu")
	res2_2b2 = Convolution("layer2_block1_sub3_sub3_conv_Conv2D", res2_2b2, 256, 1, 1, 1, 1, "SAME", "none")
	res2_2_sum = Sum("layer2_block1_shortcut_add", res2_1_sum, res2_2b2, 1.0, 1.0, "none")
	res2_2 = BatchNorm("layer3_block0_common_bn_relu_FusedBatchNorm", res2_2_sum, 0.0010000000475, "relu")
	# cfg[2]
	res3_1b2 = Convolution("layer3_block0_sub1_sub1_conv_Conv2D", res2_2, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_1b2 = Convolution("layer3_block0_sub2_sub2_conv_Conv2D", res3_1b2, 128, 3, 3, 2, 2, "SAME", "relu")
	res3_1b2 = Convolution("layer3_block0_sub3_sub3_conv_Conv2D", res3_1b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_1b1 = Convolution("layer3_block0_shortcut_sub_sc_Conv2D", res2_2, 512, 1, 1, 2, 2, "SAME", "none")
	res3_1_sum = Sum("layer3_block0_shortcut_add", res3_1b1, res3_1b2, 1.0, 1.0, "none")
	res3_1 = BatchNorm("layer3_block1_residual_bn_relu_FusedBatchNorm", res3_1_sum, 0.0010000000475, "relu")
	#
	res3_2b2 = Convolution("layer3_block1_sub1_sub1_conv_Conv2D", res3_1, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_2b2 = Convolution("layer3_block1_sub2_sub2_conv_Conv2D", res3_2b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_2b2 = Convolution("layer3_block1_sub3_sub3_conv_Conv2D", res3_2b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_2_sum = Sum("layer3_block1_shortcut_add", res3_1_sum, res3_2b2, 1.0, 1.0, "none")
	res3_2 = BatchNorm("layer3_block2_residual_bn_relu_FusedBatchNorm", res3_2_sum, 0.0010000000475, "relu")
	# block 2 to 6
	res3_3b2 = Convolution("layer3_block2_sub1_sub1_conv_Conv2D", res3_2, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_3b2 = Convolution("layer3_block2_sub2_sub2_conv_Conv2D", res3_3b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_3b2 = Convolution("layer3_block2_sub3_sub3_conv_Conv2D", res3_3b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_3_sum = Sum("layer3_block2_shortcut_add", res3_2_sum, res3_3b2, 1.0, 1.0, "none")
	res3_3 = BatchNorm("layer3_block3_residual_bn_relu_FusedBatchNorm", res3_3_sum, 0.0010000000475, "relu")
	res3_4b2 = Convolution("layer3_block3_sub1_sub1_conv_Conv2D", res3_3, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_4b2 = Convolution("layer3_block3_sub2_sub2_conv_Conv2D", res3_4b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_4b2 = Convolution("layer3_block3_sub3_sub3_conv_Conv2D", res3_4b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_4_sum = Sum("layer3_block3_shortcut_add", res3_3_sum, res3_4b2, 1.0, 1.0, "none")
	res3_4 = BatchNorm("layer3_block4_residual_bn_relu_FusedBatchNorm", res3_4_sum, 0.0010000000475, "relu")
	res3_5b2 = Convolution("layer3_block4_sub1_sub1_conv_Conv2D", res3_4, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_5b2 = Convolution("layer3_block4_sub2_sub2_conv_Conv2D", res3_5b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_5b2 = Convolution("layer3_block4_sub3_sub3_conv_Conv2D", res3_5b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_5_sum = Sum("layer3_block4_shortcut_add", res3_4_sum, res3_5b2, 1.0, 1.0, "none")
	res3_5 = BatchNorm("layer3_block5_residual_bn_relu_FusedBatchNorm", res3_5_sum, 0.0010000000475, "relu")
	res3_6b2 = Convolution("layer3_block5_sub1_sub1_conv_Conv2D", res3_5, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_6b2 = Convolution("layer3_block5_sub2_sub2_conv_Conv2D", res3_6b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_6b2 = Convolution("layer3_block5_sub3_sub3_conv_Conv2D", res3_6b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_6_sum = Sum("layer3_block5_shortcut_add", res3_5_sum, res3_6b2, 1.0, 1.0, "none")
	res3_6 = BatchNorm("layer3_block6_residual_bn_relu_FusedBatchNorm", res3_6_sum, 0.0010000000475, "relu")
	#
	res3_7b2 = Convolution("layer3_block6_sub1_sub1_conv_Conv2D", res3_6, 128, 1, 1, 1, 1, "SAME", "relu")
	res3_7b2 = Convolution("layer3_block6_sub2_sub2_conv_Conv2D", res3_7b2, 128, 3, 3, 1, 1, "SAME", "relu")
	res3_7b2 = Convolution("layer3_block6_sub3_sub3_conv_Conv2D", res3_7b2, 512, 1, 1, 1, 1, "SAME", "none")
	res3_7_sum = Sum("layer3_block6_shortcut_add", res3_6_sum, res3_7b2, 1.0, 1.0, "none")
	res3_7 = BatchNorm("layer4_block0_common_bn_relu_FusedBatchNorm", res3_7_sum, 0.0010000000475, "relu")
	# cfg[3]
	res4_1b2 = Convolution("layer4_block0_sub1_sub1_conv_Conv2D", res3_7, 256, 1, 1, 1, 1, "SAME", "relu")
	res4_1b2 = Convolution("layer4_block0_sub2_sub2_conv_Conv2D", res4_1b2, 256, 3, 3, 2, 2, "SAME", "relu")
	res4_1b2 = Convolution("layer4_block0_sub3_sub3_conv_Conv2D", res4_1b2, 1024, 1, 1, 1, 1, "SAME", "none")
	res4_1b1 = Convolution("layer4_block0_shortcut_sub_sc_Conv2D", res3_7, 1024, 1, 1, 2, 2, "SAME", "none")
	res4_1_sum = Sum("layer4_block0_shortcut_add", res4_1b1, res4_1b2, 1.0, 1.0, "none")
	res4_1 = BatchNorm("layer4_block1_residual_bn_relu_FusedBatchNorm", res4_1_sum, 0.0010000000475, "relu")
	#
	res4_2b2 = Convolution("layer4_block1_sub1_sub1_conv_Conv2D", res4_1, 256, 1, 1, 1, 1, "SAME", "relu")
	res4_2b2 = Convolution("layer4_block1_sub2_sub2_conv_Conv2D", res4_2b2, 256, 3, 3, 1, 1, "SAME", "relu")
	res4_2b2 = Convolution("layer4_block1_sub3_sub3_conv_Conv2D", res4_2b2, 1024, 1, 1, 1, 1, "SAME", "none")
	res4_2_sum = Sum("layer4_block1_shortcut_add", res4_1_sum, res4_2b2, 1.0, 1.0, "none")
	res4_2 = BatchNorm("layer4_block2_residual_bn_relu_FusedBatchNorm", res4_2_sum, 0.0010000000475, "relu")
	# ...
	res_sum = res4_2_sum
	res = res4_2
	for i in range(29):
		scope_name = "layer4_block{0}_sub1_sub1_conv_Conv2D".format(3+i)
		res_b2 = Convolution(scope_name, res, 256, 1, 1, 1, 1, "SAME", "relu")
		scope_name = "layer4_block{0}_sub2_sub2_conv_Conv2D".format(3+i)
		res_b2 = Convolution(scope_name, res_b2, 256, 3, 3, 1, 1, "SAME", "relu")
		scope_name = "layer4_block{0}_sub3_sub3_conv_Conv2D".format(3+i)
		res_b2 = Convolution(scope_name, res_b2, 1024, 1, 1, 1, 1, "SAME", "none")
		scope_name = "layer4_block{0}_shortcut_add".format(3+i)
		res_sum = Sum(scope_name, res_sum, res_b2, 1.0, 1.0, "none")
		scope_name = "layer4_block{0}_residual_bn_relu_FusedBatchNorm".format(3+i)
		res = BatchNorm(scope_name, res_sum, 0.0010000000475, "relu")
	#
	res4_32b2 = Convolution("layer4_block31_sub1_sub1_conv_Conv2D", res, 256, 1, 1, 1, 1, "SAME", "relu")
	res4_32b2 = Convolution("layer4_block31_sub2_sub2_conv_Conv2D", res4_32b2, 256, 3, 3, 1, 1, "SAME", "relu")
	res4_32b2 = Convolution("layer4_block31_sub3_sub3_conv_Conv2D", res4_32b2, 1024, 1, 1, 1, 1, "SAME", "none")
	res4_32_sum = Sum("layer4_block31_shortcut_add", res_sum, res4_32b2, 1.0, 1.0, "none")
	res4_32 = BatchNorm("layer5_block0_common_bn_relu_FusedBatchNorm", res4_32_sum, 0.0010000000475, "relu")
	# cfg[4]
	res5_1b2 = Convolution("layer5_block0_sub1_sub1_conv_Conv2D", res4_32, 512, 1, 1, 1, 1, "SAME", "relu")
	res5_1b2 = Convolution("layer5_block0_sub2_sub2_conv_Conv2D", res5_1b2, 512, 3, 3, 2, 2, "SAME", "relu")
	res5_1b2 = Convolution("layer5_block0_sub3_sub3_conv_Conv2D", res5_1b2, 2048, 1, 1, 1, 1, "SAME", "none")
	res5_1b1 = Convolution("layer5_block0_shortcut_sub_sc_Conv2D", res4_32, 2048, 1, 1, 2, 2, "SAME", "none")
	res5_1_sum = Sum("layer5_block0_shortcut_add", res5_1b1, res5_1b2, 1.0, 1.0, "none")
	res5_1 = BatchNorm("layer5_block1_residual_bn_relu_FusedBatchNorm", res5_1_sum, 0.0010000000475, "relu")
	#
	res5_2b2 = Convolution("layer5_block1_sub1_sub1_conv_Conv2D", res5_1, 512, 1, 1, 1, 1, "SAME", "relu")
	res5_2b2 = Convolution("layer5_block1_sub2_sub2_conv_Conv2D", res5_2b2, 512, 3, 3, 1, 1, "SAME", "relu")
	res5_2b2 = Convolution("layer5_block1_sub3_sub3_conv_Conv2D", res5_2b2, 2048, 1, 1, 1, 1, "SAME", "none")
	res5_2_sum = Sum("layer5_block1_shortcut_add", res5_1_sum, res5_2b2, 1.0, 1.0, "none")
	res5_2 = BatchNorm("avg_fc_fc_bn_FusedBatchNorm", res5_2_sum, 0.0010000000475, "relu")
	# avg pooling
	avg_pool = Pooling("avg_fc_AvgPool", res5_2, 7, 7, 1, 1, "AVG", "VALID")
	fc6 = Convolution("avg_fc_fc6_Conv2D", avg_pool, 6, 1, 1, 1, 1, "SAME", "none")

	return fc6


def load_weights(session, file):
	net_weights = np.fromfile(file, dtype=np.float32)
	print(net_weights.shape)

	count = 0
	for var in tf.trainable_variables():

		print(var.name)
		if -1 != var.name.find("sub3_sub3") and -1 != var.name.find("Variable_1:0"):
			print("pass")
			continue
		elif -1 != var.name.find("shortcut_sub") and -1 != var.name.find("Variable_1:0"):
			print("pass")
			continue
		elif -1 != var.name.find("fc_fc6") and -1 != var.name.find("Variable_1:0"):
			print("pass")
			continue

		weight_size = 1
		for val in var.get_shape():
			weight_size = weight_size * val

		if -1 != var.name.find("Conv") and -1 != var.name.find("Variable:0"):
			# tf - [h][w][i][o] <==> mkldnn - [o][i][h][w]
			w_shape = var.get_shape()
			w = np.reshape(net_weights[count:count+weight_size], (w_shape[3],w_shape[2],w_shape[0],w_shape[1]))
			print(w.shape)  # mkldnn tensor
			w = np.transpose(w, (2,3,1,0))  # mkldnn tensor format -> tf tensor format
			print(w.shape)  # tf tensor
			# update weights
			update = tf.assign(var, w)
			session.run(update)
		else:
			w = np.reshape(net_weights[count:count+weight_size], var.get_shape())
			update = tf.assign(var, w)
			session.run(update)

		count = count + weight_size
	print(count)


def main(argv):
	images = tf.placeholder(tf.float32, shape=(None,224,224,3))
	fc6 = create_model(images)

	init = tf.global_variables_initializer()
	with tf.Session() as sess:
		sess.run(init)
		load_weights(sess, "save_model/weights.bin")

		#saver = tf.train.Saver()
		#saver.save(sess, "tf_model/model.ckpt")

		imgs = make_fake_input(1, 224, 224, 3)
		for step in range(1):
			## dump weights
			#for var in tf.trainable_variables():
			#	if -1 != var.name.find("layer2_block0_shortcut_sub_sc"):
			#		print(var)
			#		print(var.eval())

			result = sess.run(fc6, feed_dict={images:imgs})
			print(result[0][0][0])
	return 0


if __name__ == "__main__":
	tf.app.run()
```
reference:
1. https://tensorflow.google.cn/api_docs/python/tf/nn
1. https://github.com/tensorflow/models/blob/master/official/resnet/resnet_model.py

#### 2.1 模型保存
```python
saver = tf.train.Saver()
saver.save(sess, "tf_model/model.ckpt")
```

#### 2.2 模型加载
```python
# coding:utf8
import os
import sys
import tensorflow as tf


def main():

	with tf.Session() as sess:
		saver = tf.train.import_meta_graph("tf_model/model.ckpt.meta")
		saver.restore(sess, tf.train.latest_checkpoint("tf_model/"))

		for var in tf.trainable_variables():
			print(var.name)
			print(var.eval())
			break

	return 0


if __name__ == "__main__":
	main()
```


#### 3. supplement knowledge
###### **1. BatchNorm**
**▪ tf.nn:** <br>
<img src="pics/batchNorm.jpg" width="50%"> <br>
variance_epsilon: A small float number to avoid dividing by 0. <br>
**▪ mkldnn:** <br>
<img src="pics/batchNorm_mkl.jpg" width="65%"> <br>

###### **2. tf.add(a, b) 与 a+b**
在神经网络前向传播的过程中，经常可见如下两种形式的代码:
* tf.add(tf.matmul(x, w), b)
* tf.matmul(x, w) + b

二者并没有差别. 运算符重载的形式 a+b, 会在内部转换为 a.\_\_add__(b), 而 a.\_\_add__(b) 会再一次地映射为 tf.add.

## Tensorflow - High Level Library
* Keras
* TensorLayer
* TFLearn
* tensorflow.contrib.slim


## Tensorflow - use high level library to create your cnn
Keras
```python

```
