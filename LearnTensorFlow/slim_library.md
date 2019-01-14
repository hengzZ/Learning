# slim library learning note
TF-Slim is a lightweight library for defining, training and evaluating complex models in TensorFlow. Components of tf-slim can be freely mixed with native tensorflow, as well as other frameworks, such as tf.contrib.learn.

## Components
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
#### 1. argument scope
In addition to the types of **scope mechanisms** in TensorFlow (*name_scope*, *variable_scope*), TF-Slim adds a new scoping mechanism called *arg_scope*. This new scope allows a user to specify one or more operations and a set of arguments which will be passed to each of the operations defined in the arg_scope.
```python
## version 1
net = slim.conv2d(inputs, 64, [11, 11], 4, padding='SAME',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv1')
net = slim.conv2d(net, 128, [11, 11], padding='VALID',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv2')
net = slim.conv2d(net, 256, [11, 11], padding='SAME',
                  weights_initializer=tf.truncated_normal_initializer(stddev=0.01),
                  weights_regularizer=slim.l2_regularizer(0.0005), scope='conv3')

## version 2
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

## This solution ensures that all three convolutions share the exact same parameter values but doesn't reduce completely the code clutter. By using an arg_scope, we can both ensure that each layer uses the same values and simplify the code:


## version 3
with slim.arg_scope([slim.conv2d], padding='SAME',
                      weights_initializer=tf.truncated_normal_initializer(stddev=0.01)
                      weights_regularizer=slim.l2_regularizer(0.0005)):
    net = slim.conv2d(inputs, 64, [11, 11], scope='conv1')
    net = slim.conv2d(net, 128, [11, 11], padding='VALID', scope='conv2')
    net = slim.conv2d(net, 256, [11, 11], scope='conv3')

```

#### 2. weight regularizer


###### reference:
https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/slim
