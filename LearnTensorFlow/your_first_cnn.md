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
This network is a modified version of resnet v2 (full pre-activation) <br>
<img src="pics/resnet_v2.JPG" width="40%"> <br>
Reference: Identity Mappings in Deep Residual Networks (http://arxiv.org/pdf/1603.05027.pdf)

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


def get_parser():
	"""
	create a parser to parse argument "--cpu_num --inter_op_threads --intra_op_threads"
	"""
	parser = argparse.ArgumentParser(description="Specify tensorflow parallelism")
	parser.add_argument("--cpu_num", dest="cpu_num", default=1, help="specify how many cpus to use.(default: 1)")
	parser.add_argument("--inter_op_threads", dest="inter_op_threads", default=1, help="specify max inter op parallelism.(default: 1)")
	parser.add_argument("--intra_op_threads", dest="intra_op_threads", default=1, help="specify max intra op parallelism.(default: 1)")
	parser.add_argument("--dump_timeline", dest="dump_timeline", default=False, help="specify to dump timeline.(default: False)")
	return parser


def main(argv):
	parser = get_parser()
	args = parser.parse_args()
	#parser.print_help()
	cpu_num = int(args.cpu_num)
	inter_op_threads = int(args.inter_op_threads)
	intra_op_threads = int(args.intra_op_threads)
	dump_timeline = bool(args.dump_timeline)
	# dump parallelism settings
	print("cpu_num: ", cpu_num)
	print("inter_op_threads: ", inter_op_threads)
	print("intra_op_threads: ", intra_op_threads)
	print("dump_timeline: ", dump_timeline)

	images = tf.placeholder(tf.float32, shape=(None,224,224,3))
	fc6 = create_model(images)

	init = tf.global_variables_initializer()
	
	config = tf.ConfigProto(device_count={"CPU": 4}, # limit to num_cpu_core CPU usage
                inter_op_parallelism_threads = 1, # OP 之间并行度(针对当运算符op为单一运算符，并且内部可以实现并行时，如矩阵乘法)
                intra_op_parallelism_threads = 4, # OP 内部并行度(针对当有多个运算符op，并且他们之间比较独立，运算符和运算符之间没有直接的路径Path相连.即并行网络分支)
                log_device_placement=False)
	with tf.Session(config = config) as sess:
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
			if dump_timeline:
				run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
				run_metadata = tf.RunMetadata()
				result = sess.run(fc6, feed_dict={images:imgs}, options=run_options, run_metadata=run_metadata)
				tm = timeline.Timeline(run_metadata.step_stats)
				ctf = tm.generate_chrome_trace_format()
				with open('timeline.json', 'w') as f:
					f.write(ctf)
			else:
				result = sess.run(fc6, feed_dict={images:imgs})
			print(result[0][0][0])
	return 0


if __name__ == "__main__":
	tf.app.run()
```
reference:
1. https://tensorflow.google.cn/api_docs/python/tf/nn
1. https://github.com/tensorflow/models/blob/master/official/resnet/resnet_model.py

###### **timeline 数据查**
* step 1: 将目录中 timeline.json 拷贝至windows
* step 2: 打开 chrome 浏览器，输入：chrome://tracing
* step 3: 点击 load 按钮，导入 timeline.json 文件查看


#### 2.1 模型保存
```python
# 请记住，Tensorflow 变量仅在会话中存在! 因此，必须在一个会话中保存模型。
saver = tf.train.Saver()
saver.save(sess, "tf_model/model.ckpt")
```
结果：
```bash
checkpoint
model.ckpt.data-00000-of-00001
model.ckpt.index
model.ckpt.meta
```
* meta graph - 这是一个协议缓冲区，它保存了完整的 tensorflow 图形。即所有变量、操作、集合等。该文件以 .meta 作为扩展名。
* checkpoint file:
	* tensorflow 0.11 版本以前，这是一个二进制文件，它包含了所有的权重、偏差、梯度和其他所有变量的值。扩展名 .ckpt。
	* 现在，有两个文件，而不是单个 .ckpt 文件。 （分别为 index 和 data 文件，另外一个名为 checkpoint 的文件，它只保存最新保存的 checkpoint 文件的记录。）

#### 2.2 模型加载
如果你想用别人预先训练好的模型，需要做以下两件事：
* 创建网络 - 可以通过编写 python 代码创建网络，以手工创建每一层，并将其作为原始模型。 如果已经在 .meta 文件中保存了这个网络，可以使用 tf.train.import() 函数来重新创建这个网络。
```python
saver = tf.train.import_meta_graph("model/model.ckpt.meta")
```
* 载入参数 - 恢复网络的参数
```python
saver.restore(sess, tf.train.latest_checkpoint("model/"))
```

示例：
```python
# coding:utf8
import tensorflow as tf


def main():

	with tf.Session() as sess:
		saver = tf.train.import_meta_graph("model/model.ckpt.meta")
		saver.restore(sess, tf.train.latest_checkpoint("model/"))

		for var in tf.trainable_variables():
			print(var.name)
			print(var.eval())
			break

	return 0


if __name__ == "__main__":
	main()
```

#### 2.3 使用导入的模型
* 当我们想要恢复一个模型时，不仅要恢复图和权重，还要准备一个新的feed_dict，它将把新的数据输入到网络中。
* 如果希望将新的训练数据更新到网络中，可以通过 graph.get_tensor_by_name() 方法来引用保存的操作和占位符变量。
```python
#How to access saved variable/Tensor/placeholders 
w1 = graph.get_tensor_by_name("w1:0")

## How to access saved operation
op_to_restore = graph.get_tensor_by_name("op_to_restore:0")
```
* 如果只是想用不同的数据运行相同的网络，只需简单地通过 feed_dict 将新数据传递给网络。
```python
import tensorflow as tf
 
sess=tf.Session()    
# 第一步: let's load meta graph and restore weights
saver = tf.train.import_meta_graph('my_test_model-1000.meta')
saver.restore(sess,tf.train.latest_checkpoint('./'))
 
 
# 第二步： let's access and create placeholders variables and create feed-dict to feed new data 
graph = tf.get_default_graph()
w1 = graph.get_tensor_by_name("w1:0")
w2 = graph.get_tensor_by_name("w2:0")
feed_dict ={w1:13.0,w2:17.0}
 
#第三步： access the op that you want to run. 
op_to_restore = graph.get_tensor_by_name("op_to_restore:0")

# 运行
print sess.run(op_to_restore,feed_dict)
# Note: This will print 60 which is calculated using new values of w1 and w2 and saved value of b1. 
```

示例：
```python
# coding:utf8
import numpy as np
import tensorflow as tf

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

def main():

	imgs = make_fake_input(1, 224, 224, 3)

	with tf.Session() as sess:
		saver = tf.train.import_meta_graph("model/model.ckpt.meta")
		saver.restore(sess, tf.train.latest_checkpoint("model/"))

		#variable_names = [v.name for v in tf.trainable_variables()]
		#variable_names = [v.name for v in tf.global_variables()]
		#values = sess.run(variable_names)
		#
		#for k,v in zip(variable_names, values):
		#       print("Variable Name: ", k)
		#       print("Shape: ", v.shape)
		#       #print(v)

		## Nodes
		#for n in tf.get_default_graph().as_graph_def().node:
		#       print(n)

		## Operations
		#for op in tf.get_default_graph().get_operations():
		#       print(op.name)
		#       print(op.values())

		## Variables
		#for variable in tf.all_variables():
		#       print(variable)
		#       print(variable.name)

		graph = tf.get_default_graph()
		input = graph.get_tensor_by_name("Placeholder:0")  ## find this tensor name by dumping the op.values()
		fc6 = graph.get_tensor_by_name("avg_fc_fc6_Conv2D/BiasAdd:0")  ## find this tensor name by dumping op.values()

		result = sess.run(fc6, feed_dict={input:imgs})  ## 特别注意， sess.run 传入的是 tensor 对象，网络层定义时返回的也是 tensor 对象!!
		print(result)

	return 0

if __name__ == "__main__":
	main()
```
Operations 详情信息如下：
```python
Placeholder
(<tf.Tensor 'Placeholder:0' shape=(?, 224, 224, 3) dtype=float32>,)
...
avg_fc_fc6_Conv2D/Variable_1/read
(<tf.Tensor 'avg_fc_fc6_Conv2D/Variable_1/read:0' shape=(6,) dtype=float32>,)
avg_fc_fc6_Conv2D/BiasAdd
(<tf.Tensor 'avg_fc_fc6_Conv2D/BiasAdd:0' shape=(?, 1, 1, 6) dtype=float32>,)
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

###### **3. 获取计算图中的 variable 和 operation
**Part 1: 获取某一个 variable 和 operation**
```python
graph = tf.get_default_graph()
graph.get_tensor_by_name(tensor_name)
graph.get_operation_by_name(op_name)
```
**Part 2: 遍历方式获取 operation** <br>
方法1： **node** 节点
```python
# 该方法可列出 graph 中每个 node 的详细信息
for n in tf.get_default_graph().as_graph_def().node:
	print(n)
```
方法2： **operation** 变量
```python
# op.valuses() 将返回 op 对应的 tensor 对象，可以进一步获取 tensor 的 name, shape 等信息
for op in tf.get_default_graph().get_operations():
	print(op.name)
	print(op.values())
```
**Part 3: 遍历方式获取 variable** <br>
方法1： tf.all_variables()
```python
# 显示默认计算图中所有的 variable（） 对象
for variable in tf.all_variables():
    print variable
    print variable.name
```
方法2： tf.get_collection(collection_key)
```python
# 根据 key 返回相应 collection 中的对象, tensorflow 中预定义了一些 key, 如 GLOBAL_VARIABLES, LOCAL_VARIABLES, TRAINABLE_VARIABLES 等
for variable in tf.get_collection(tf.GraphKeys.GLOBAL_VARIABLES):
	print variable
```

#### 使用 slim 库的 model_analyzer 打印权重信息
```python
model_vars = tf.trainable_variables()
slim.model_analyzer.analyze_vars(model_vars, print_info=True)
```
**理清完整的网络拓扑信息，请结合 nodes 信息， ops 信息和 slim 打印的权重信息。**


## Tensorflow - High Level Library
* Keras
* TensorLayer
* TFLearn
* tensorflow.contrib.slim


## Tensorflow - use high level library to create your cnn
Keras
```python

```
