## TensorFlow 模型格式
### 1. CheckPoint(*.ckpt)
训练过程中每迭代若干轮保存一次权值到磁盘，称为 “checkpoint”。
```bash
$ ls
model.ckpt-120000.data-00000-of-00001
model.ckpt-120000.index
model.ckpt-120000.meta
```
checkpoint 由 tf.train.Saver() 对象调用 saver.save() 生成，其只包含若干 Variables 对象序列化后的数据，不包含图结构，所以只给 checkpoint 模型不提供代码是无法重新构建计算图的。

### 2. GraphDef(*.pb)
pb 文件包含 protobuf 对象序列化后的数据，包含了计算图，可以从中得到所有运算符（operators）的细节，也包含张量（tensors）和 Variables 定义，但不包含 Variable 的值，因此只能从中恢复计算图，权值仍需要从 checkpoint 中恢复。
```bash
$ ls
frozen_inference_graph.pb
```
冻结（frozen）：FrozenGraphDef 格式。这种文件格式不包含 Variables 节点，将 GraphDef 中所有 Variable 节点转换为常量（其值从 checkpoint 获取），就变为 FrozenGraphDef 格式。<br>
*.pb 为二进制文件，实际上 protobuf 也支持文本格式（*.pbtxt）。

### 3. SavedModel
该格式为 GraphDef 和 CheckPoint 的结合体，另外还有标记模型输入和输出参数的 SignatureDef。从 SavedModel 中可以提取 GraphDef 和 CheckPoint 对象。
```bash
assets/
assets.extra/
variables/
    variables.data-????-of-????
    variables.index
saved_model.pd
```
saved_model.pb（或 saved_model.pbtxt）包含使用 MetaGraphDef protobuf 对象定义的计算图；assets 包含附加文件；variables 目录包含 tf.train.Saver() 对象调用 save() API 生成的权值文件。


## 示例1. ckpt 和 pd 模型初始化
```python
#!/usr/bin/python
import sys
import os
import tensorflow as tf


def main():
        prototxt = "./frozen_inference_graph.pb"
        weights = "./checkpoint/model.ckpt-120000.data-00000-of-00001"
        ckpt_meta = "./checkpoint/model.ckpt-120000.meta"
        with tf.Session() as sess:
                # way 1: load model from pb file
                model_file = tf.gfile.FastGFile(prototxt)
                graph_def = tf.GraphDef()
                graph_def.ParseFromString(model_file.read())
                tf.import_graph_def(graph_def, name="")
                # way 2: load model from ckpt_meta
                #tf.train.import_meta_graph(ckpt_meta)
                # get graph
                graph = tf.get_default_graph()
                # export graph's structure
                summary_write = tf.summary.FileWriter("./" , graph)
        return 0


if __name__ == "__main__":
        sys.exit(main())
```


## 示例2. OpenVINO 适配 TensorFlow 模型

#### 情形1. Load Non-Frozen Models to the Model Optimizer (OpenVINO)
三种 Non-Frozen 模型：
* Checkpoint （两个文件）
    * inference_graph.pb or inference_graph.pbtxt （模型）
    * checkpoint_file.ckpt （权重）
* MetaGraph （三或四个文件）
    * model_name.meta
    * model_name.index
    * model_name.data-00000-of-00001 (digit part may vary)
    * checkpoint (optional)
* SavedModel - a special directory with a .pb file and several subfolders: variables, assets, and assets.extra.

#### 情形2. Load Frozen Models to the Model Optimizer (OpenVINO)

##### Step1. Freeze Custom Models in Python
###### When a network is defined in Python* code, such as graphs are built in a form that allows model training. That means that all trainable parameters are represented as variables in the graph. To use the graph with the Model Optimizer, it should be frozen (***save the graph into a file***).

```python
# 将模型保存为 frozen 形式
import tensorflow as tf
from tensorflow.python.framework import graph_io

frozen = tf.graph_util.convert_variables_to_constants(sess, sess.graph_def, ["<name_of_the_output_node>"])
graph_io.write_graph(frozen, './', 'inference_graph.pb', as_text=False)
```

##### Step2. Convert TF model to OpenVino IR (Intermediate  Representation)
```bash
python3 mo_tf.py --input_model <INPUT_MODEL>.pb
```
