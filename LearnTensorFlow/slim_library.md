# Slim Library Learning Note
TF-Slim is a lightweight library for defining, training and evaluating complex models in TensorFlow. Components of tf-slim can be freely mixed with native tensorflow, as well as other frameworks, such as tf.contrib.learn.

## Slim - Components
TF-Slim is composed of several parts which were design to exist independently. These include the following main pieces:

* **arg_scope**: provides a new scope named arg_scope that allows a user to define default arguments *for specific operations within that scope*.
* data: contains TF-slim's dataset definition, data providers, parallel_reader, and decoding utilities.
* evaluation: contains routines for evaluating models.
* **layers**: contains high level layers for building models using tensorflow.
* learning: contains routines for training models.
* losses: contains commonly used loss functions.
* metrics: contains popular evaluation metrics.
* **nets**: contains popular network definitions such as VGG and AlexNet models.
* queues: provides a context manager for easily and safely starting and closing QueueRunners.
* **regularizers**: contains weight regularizers.
* **variables**: provides convenience wrappers for variable creation and manipulation.


## Puzzles
#### 1. Argument Scope
*In addition to the types of **scope mechanisms** in TensorFlow (**name_scope**, **variable_scope**), TF-Slim adds a new scoping mechanism called **arg_scope**. This new scope allows a user to specify one or more operations and a set of arguments which will be passed to each of the operations defined in the arg_scope.*
```python
## version 1 - original code
net = slim.conv2d(inputs, 64, [11, 11], 4, padding='SAME',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv1')
net = slim.conv2d(net, 128, [11, 11], padding='VALID',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv2')
net = slim.conv2d(net, 256, [11, 11], padding='SAME',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv3')

## version 2 - use variable instead of repeated values
padding = 'SAME'
initializer = tf.truncated_normal_initializer(stddev=0.01)
regularizer = slim.l2_regularizer(0.0005)
net = slim.conv2d(inputs, 64, [11, 11], 4,
                  padding=padding,
                  weights_initializer=initializer,
                  weights_regularizer=regularizer,
                  scope='conv1')
net = slim.conv2d(net, 128, [11, 11],
                  padding='VALID',
                  weights_initializer=initializer,
                  weights_regularizer=regularizer,
                  scope='conv2')
net = slim.conv2d(net, 256, [11, 11],
                  padding=padding,
                  weights_initializer=initializer,
                  weights_regularizer=regularizer,
                  scope='conv3')

## This solution ensures that all three convolutions share the exact same parameter values
## but doesn't reduce completely the code clutter.


## version 3 - scope mechanism
with slim.arg_scope([slim.conv2d], padding='SAME',
                      weights_initializer=tf.truncated_normal_initializer(stddev=0.01)
                      weights_regularizer=slim.l2_regularizer(0.0005)):
    net = slim.conv2d(inputs, 64, [11, 11], scope='conv1')
    net = slim.conv2d(net, 128, [11, 11], padding='VALID', scope='conv2')
    net = slim.conv2d(net, 256, [11, 11], scope='conv3')

## As the example illustrates, the use of arg_scope makes the code cleaner, simpler and easier to maintain.
## Notice that while argument values are specified in the arg_scope, they can be overwritten locally.


# Example: **Nest arg_scopes**
with slim.arg_scope([slim.conv2d, slim.fully_connected],
                      activation_fn=tf.nn.relu,
                      weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                      weights_regularizer=slim.l2_regularizer(0.0005)):
  with slim.arg_scope([slim.conv2d], stride=1, padding='SAME'):
    net = slim.conv2d(inputs, 64, [11, 11], 4, padding='VALID', scope='conv1')
    net = slim.conv2d(net, 256, [5, 5],
                      weights_initializer=tf.truncated_normal_initializer(stddev=0.03),
                      scope='conv2')
    net = slim.fully_connected(net, 1000, activation_fn=None, scope='fc')
```

#### 2. Weight Regularizer
*L1, L2 (L2 regularization is also called **weight decay**)，作用于模型训练时的参数更新。*<br>
online book: Neural Networks and Deep Learning (http://neuralnetworksanddeeplearning.com)

防止过拟合，提高泛化能力 -> Regularization、Dropout、Data Augmentation

<br>
<br>

* 人们普遍认为：更小的权值w，从某种意义上说，表示网络的复杂度更低，对数据的拟合刚刚好（这个法则也叫做奥卡姆剃刀）
* **奥卡姆剃刀原理 - 如无必要，勿增实体。（即“简单有效原理”）**

<br>
<br>

#### 3. Create Layers
orignal tensorflow: <br>
*A layer, such as a Convolutional Layer, a Fully Connected Layer or a BatchNorm Layer are more abstract than a single TensorFlow operation and typically involve several operations*
```python
input = ...
with tf.name_scope('conv1_1') as scope:
  kernel = tf.Variable(tf.truncated_normal([3, 3, 64, 128], dtype=tf.float32,
                                           stddev=1e-1), name='weights')
  conv = tf.nn.conv2d(input, kernel, [1, 1, 1, 1], padding='SAME')
  biases = tf.Variable(tf.constant(0.0, shape=[128], dtype=tf.float32),
                       trainable=True, name='biases')
  bias = tf.nn.bias_add(conv, biases)
  conv1 = tf.nn.relu(bias, name=scope)
```

TF-Slim provides a number of convenient operations defined at the more abstract level of neural network layers:
```python
# create a 'conv1_1' layer same as above
input = ...
net = slim.conv2d(input, 128, [3, 3], scope='conv1_1')
```

TF-Slim also provides two *meta-operations* called **repeat** and **stack** that allow users to repeatedly perform the same operation:
```python
[Example 1:]
# version 1 - direct way
net = ...
for i in range(3):
  net = slim.conv2d(net, 256, [3, 3], scope='conv3_%d' % (i+1))
net = slim.max_pool2d(net, [2, 2], scope='pool2')

# version 2 - repeat meta-operation
net = slim.repeat(net, 3, slim.conv2d, 256, [3, 3], scope='conv3')
net = slim.max_pool2d(net, [2, 2], scope='pool2')

## Note: slim.repeat - allows a caller to repeatedly apply the same operation with same argument !!

[Example 2:]
# version 1 - verbose way
x = slim.fully_connected(x, 32, scope='fc/fc_1')
x = slim.fully_connected(x, 64, scope='fc/fc_2')
x = slim.fully_connected(x, 128, scope='fc/fc_3')

# version2 - stack meta-operation
slim.stack(x, slim.fully_connected, [32, 64, 128], scope='fc')

## Note: slim.stack - allows a caller to repeatedly apply the same operation with different arguments !!
```

A tower of multiple convolutions:
```python
# Verbose way:
x = slim.conv2d(x, 32, [3, 3], scope='core/core_1')
x = slim.conv2d(x, 32, [1, 1], scope='core/core_2')
x = slim.conv2d(x, 64, [3, 3], scope='core/core_3')
x = slim.conv2d(x, 64, [1, 1], scope='core/core_4')

# Using stack:
slim.stack(x, slim.conv2d, [(32, [3, 3]), (32, [1, 1]), (64, [3, 3]), (64, [1, 1])], scope='core')
```

#### 4. Slim Built-In Networks
官方代码（发布版）： http://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/slim （from tensorflow.contrib.slim.nets import resnet_v2） <br>
官方代码（测试版）： http://github.com/tensorflow/models/blob/master/research/slim/nets/resnet_v2.py

代码追踪：
* slim 包（**package**）初始化： models/research/slim/\_\_init__.py 或者 tensorflow/contrib/slim/\_\_init__.py
* nets 模块（**module**）初始化： models/research/slim/nets/\_\_init__.py 或者 tensorflow/contrib/slim/nets.py
* resnet_v2 源码： models/research/slim/nets/resnet_v2.py 或者 tensorflow/contrib/slim/python/slim/nets/resnet_v2.py

##### 补充 - python 代码结构
	* package - A dir with __init__.py
	
	* module - You can use any Python source file as a module 
	           by executing an import statement in some other Python source file.
	
	* namespace
	
	Python's *from...import* lets you import specific attributes from a module into the current namespace

*Simply, a python module is a file consisting of Python code. A module can define functions, classes and variables. A module can also include runnable code.*

## Slim - Use Built-In Resnet_v2
```python
# coding:utf8
import os
import sys

import numpy as np
import tensorflow as tf
slim = tf.contrib.slim

from tensorflow.contrib.slim.nets import resnet_v2  ## tensorflow/contrib/slim/nets.py

from tensorflow.python.client import timeline
import argparse
import time


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


def main():

        parser = get_parser()
        args = parser.parse_args()
        #parser.print_help()
        cpu_num = int(args.cpu_num)
        inter_op_threads = int(args.inter_op_threads)
        intra_op_threads = int(args.intra_op_threads)
        dump_timeline = bool(args.dump_timeline)
        print("cpu_num: ", cpu_num)
        print("inter_op_threads: ", inter_op_threads)
        print("intra_op_threads: ", intra_op_threads)
        print("dump_timeline: ", dump_timeline)

        # create model
        inputs = tf.placeholder(tf.float32, shape=(None,224,224,3))

        arg_scope = resnet_v2.resnet_arg_scope()
        with slim.arg_scope(arg_scope):
                #net, endpoints = resnet_v2.resnet_v2_152(inputs, 1000, is_training=False)
                net, endpoints = resnet_v2.resnet_v2_101(inputs, 6, is_training=False)

                ## ResNet-101 for semantic segmentation into 21 classes:
                # net, end_points = resnet_v2.resnet_v2_101(inputs,
                #                                 21,
                #                                 is_training=False,
                #                                 global_pool=False,
                #                                 output_stride=16)


        # create initialization op
        init = tf.global_variables_initializer()

        config = tf.ConfigProto(device_count={"CPU": cpu_num}, # limit to num_cpu_core CPU usage
                inter_op_parallelism_threads = inter_op_threads,
                intra_op_parallelism_threads = intra_op_threads,
                log_device_placement=False)
        with tf.Session(config = config) as sess:
                sess.run(init)
                imgs = make_fake_input(1, 224, 224, 3)

                #for op in tf.get_default_graph().get_operations():
                #       print(op.name)
                #       print(op.values())

                time_start = time.time()
                for step in range(200):
                        if dump_timeline:
                                run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
                                run_metadata = tf.RunMetadata()
                                result = sess.run(net, feed_dict={inputs:imgs}, options=run_options, run_metadata=run_metadata)
                                tm = timeline.Timeline(run_metadata.step_stats)
                                ctf = tm.generate_chrome_trace_format()
                                with open('timeline.json', 'w') as f:
                                        f.write(ctf)
                                print(result)
                        else:
                                result = sess.run(net, feed_dict={inputs:imgs})
                                print(result)
                time_end = time.time()
                avg_time = (time_end-time_start) * 1000 / 200;
                print("AVG Time: ", avg_time, " ms")
        return 0


if __name__ == "__main__":
        sys.exit(main())
```


## Slim - Create VGG16
```python
def vgg16(inputs):
  with slim.arg_scope([slim.conv2d, slim.fully_connected],
                      activation_fn=tf.nn.relu,
                      weights_initializer=tf.truncated_normal_initializer(0.0, 0.01),
                      weights_regularizer=slim.l2_regularizer(0.0005)):
    net = slim.repeat(inputs, 2, slim.conv2d, 64, [3, 3], scope='conv1')
    net = slim.max_pool2d(net, [2, 2], scope='pool1')
    net = slim.repeat(net, 2, slim.conv2d, 128, [3, 3], scope='conv2')
    net = slim.max_pool2d(net, [2, 2], scope='pool2')
    net = slim.repeat(net, 3, slim.conv2d, 256, [3, 3], scope='conv3')
    net = slim.max_pool2d(net, [2, 2], scope='pool3')
    net = slim.repeat(net, 3, slim.conv2d, 512, [3, 3], scope='conv4')
    net = slim.max_pool2d(net, [2, 2], scope='pool4')
    net = slim.repeat(net, 3, slim.conv2d, 512, [3, 3], scope='conv5')
    net = slim.max_pool2d(net, [2, 2], scope='pool5')
    net = slim.fully_connected(net, 4096, scope='fc6')
    net = slim.dropout(net, 0.5, scope='dropout6')
    net = slim.fully_connected(net, 4096, scope='fc7')
    net = slim.dropout(net, 0.5, scope='dropout7')
    net = slim.fully_connected(net, 1000, activation_fn=None, scope='fc8')
  return net
```

###### reference:
slim manual: http://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/slim

