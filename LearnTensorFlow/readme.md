# TensorFlow Quick Start

## Install tensorflow
```bash
# tensorflow-gpu for GPU support
pip install tensorflow --proxy="proxy_addr:port"
```
note: proxy is optional.
#### Update or change the version of tensorflow
```bash
# please uninstall the old version tensorflow as below:
pip uninstall tensorflow
```


## Your first TF program
```python
#!/usr/bin/python
import tensorflow as tf
import numpy as np


if __name__ == "__main__":
    matrix1 = tf.constant([[3,3]])  # 1x2 matrix - (3,3)
    matrix2 = tf.constant([[2], [2]])  # 2x1 matrix - (2,2)
    product = tf.matmul(matrix1,matrix2)  # Multiplies matrix a by matrix b

    with tf.Session() as sess:
        result2 = sess.run(product)
        print(result2)
```
因为 product 不是直接计算的步骤, 所以我们会要使用 Session 来激活 product 并得到计算结果.


## 张量（Tensor)
张量有多种:
* 零阶张量为 纯量或标量 (scalar) 也就是一个数值. 比如 [1]
* 一阶张量为 向量 (vector), 比如 一维的 [1, 2, 3]
* 二阶张量为 矩阵 (matrix), 比如 二维的 [[1, 2, 3],[4, 5, 6],[7, 8, 9]]
* 以此类推, 还有 三阶 三维的 …


## TensorFlow - Create your first model
**Target Model:** <br>
```python
y = x*0.1 + 0.3
```
Now, There are 3 steps to do:
* Create training data
* Create a model with variable parameters
* Train the model
```python
#!/usr/bin/python
import os
import sys

import tensorflow as tf
import numpy as np

def main():
    # create training data
    x_data = np.random.rand(100).astype(np.float32)
    y_data = x_data*0.1 + 0.3
    # create a model
    Weights = tf.Variable(tf.random_uniform([1], -1.0, 1.0))
    biases = tf.Variable(tf.zeros([1]))
    y = Weights*x_data + biases
    # train the model
    ## first, please define the loss function
    loss = tf.reduce_mean(tf.square(y-y_data))
    ## second, please define a solver/optimizer
    optimizer = tf.train.GradientDescentOptimizer(0.5)
    train = optimizer.minimize(loss)

    # Do training
    init = tf.global_variables_initializer()
    with tf.Session() as sess:
        sess.run(init)  # initialize the model parameters
        for step in range(201):
            sess.run(train)  # do fw/bw one time
            if step%20 == 0:
                print(step, sess.run(Weights), sess.run(biases))  # present the parameters
    return 0


if __name__ == "__main__":
    sys.exit(main())
```


## TensorFlow - Variable
在 Tensorflow 中，使用变量前首先要定义变量，这一点是与 Python 的变量使用理念不同。
```python
import tensorflow as tf

# define a variable
state = tf.Variable(0, name='counter')
# define a constant
one = tf.constant(1)

# define an add operator
new_value = tf.add(state, one)
# define an assign operator
update = tf.assign(state, new_value)

# define a initializer if you define any variables
init = tf.global_variables_initializer()

# run
with tf.Session() as sess:
    sess.run(init)
    for _ in range(3):
        sess.run(update)
        print(sess.run(state))
```
在 TensorFlow 中，变量（variable）、常量(constant)、操作符(operator)、初始化操作（initialize）等等任何的值和操作在使用之前都要首先声明！!


## TensorFlow - Placeholder
使用 Tensorflow 时，如果想要从外部传入data，那就需要用到 `tf.placeholder()`，然后以这种形式传输数据 `sess.run(***, feed_dict={input: **})`。
```python
import tensorflow as tf

input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)

ouput = tf.multiply(input1, input2)

# run
with tf.Session() as sess:
    # sess.run(ouput, feed_dict={input1: [7.], input2: [2.]})
    print(sess.run(ouput, feed_dict={input1: [7.], input2: [2.]}))
```


## TensorFlow - Tensorboard
```python
/* File Name : tb_test.py */
# coding:utf8
import os
import sys

import tensorflow as tf
import numpy as np


def add_layer(inputs, in_size, out_size, activation_function=None):
    # add one more layer and return the output of this layer
    with tf.name_scope('layer'):
        with tf.name_scope('weights'):
            Weights = tf.Variable(
                tf.random_normal([in_size, out_size]),
                name='W')
        with tf.name_scope('biases'):
            biases = tf.Variable(
                tf.zeros([1, out_size]) + 0.1,
                name='b')
        with tf.name_scope('Wx_plus_b'):
            Wx_plus_b = tf.add(
                tf.matmul(inputs, Weights),
                biases)
        if activation_function is None:
            outputs = Wx_plus_b
        else:
            outputs = activation_function(Wx_plus_b, )
        return outputs


def main():
    with tf.name_scope('inputs'):
        # define placeholder for inputs to network
        xs = tf.placeholder(tf.float32, [None, 1])
        ys = tf.placeholder(tf.float32, [None, 1])

    l1 = add_layer(xs, 1, 10, activation_function=tf.nn.relu)
    prediction = add_layer(l1, 10, 1, activation_function=None)

    # the error between prediciton and real data
    with tf.name_scope('loss'):
        loss = tf.reduce_mean(
            tf.reduce_sum(
                tf.square(ys - prediction)
            ))

    with tf.name_scope('train'):
        train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)

    sess = tf.Session()  # get session
    # tf.train.SummaryWriter soon be deprecated, use following
    writer = tf.summary.FileWriter("logs/", sess.graph)
    return 0


if __name__ == "__main__":
    sys.exit(main())
```
使用 with tf.name_scope('inputs') 可以将 xs 和 ys 包含进来，形成一个大的图层，图层的名字就是 with tf.name_scope() 方法里的参数。

**run:**
```bash
python tb_test.py
tensorboard --logdir logs
```


## TensorFlow - Saver
##### Save model
```python
import sys
import os
import tensorflow as tf
import numpy as np


def main():
    ## Save to file
    # remember to define the same dtype and shape when restore
    W = tf.Variable([[1, 2, 3], [3, 4, 5]], dtype=tf.float32, name='weights')
    b = tf.Variable([[1, 2, 3]], dtype=tf.float32, name='biases')
    # initialize the variables
    init = tf.global_variables_initializer()

    #
    saver = tf.train.Saver()
    with tf.Session() as sess:
        sess.run(init)
        save_path = saver.save(sess, "my_net/save_net.ckpt")
        print("Save to path: ", save_path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
```
保存模型，首先要建立一个 tf.train.Saver() 用来保存，提取变量。再创建一个文件夹，将 checkpoint 保存于文件夹内。
##### Restore model
```python
import sys
import os
import tensorflow as tf
import numpy as np


def main():
    # First, create the containers of  W and b
    W = tf.Variable(np.arange(6).reshape((2, 3)), dtype=tf.float32, name="weights")
    b = tf.Variable(np.arange(3).reshape((1, 3)), dtype=tf.float32, name="biases")

    # there no need the step: init = tf.global_variables_initializer()

    saver = tf.train.Saver()
    with tf.Session() as sess:
        # restore the variables
        saver.restore(sess, "my_net/save_net.ckpt")
        print("weights:", sess.run(W))
        print("biases:", sess.run(b))
    return 0


if __name__ == "__main__":
    sys.exit(main())
```
note: Remember to define the same dtype and shape when restoring.


# 迁移学习
对于一个有分类能力的神经网络, 有时候我们只需要这套神经网络的理解能力, 并拿这种能力去处理其他问题. 所以我们保留它的代表特征转换能力. 因为有了这种能力, 就能将复杂的图片像素信息转换成更少量, 但更精辟的信息. 然后我们需要干点坏事, 将这个神经网络的输出层给拆掉. 套上另外一个神经网络, 用这种移植的方式再进行训练, 让它处理不同的问题.
