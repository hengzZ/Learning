# Tensorflow / MKLDNN 拓扑结构查看

## 1 Tensorflow
参考：
* TensorFlow 模型格式 https://github.com/hengzZ/Learning/blob/master/LearnTensorFlow/tensorflow_model.md
* Your First TF CNN https://github.com/hengzZ/Learning/blob/master/LearnTensorFlow/your_first_cnn.md

```python
variable_names = [v.name for v in tf.trainable_variables()]
variable_names = [v.name for v in tf.global_variables()]
values = sess.run(variable_names)

for k,v in zip(variable_names, values):
    print("Variable Name: ", k)
    print("Shape: ", v.shape)
    #print(v)

# Nodes
# 该方法可列出 graph 中每个 node 的详细信息
for n in tf.get_default_graph().as_graph_def().node:
    print(n)

# Operations
# op.valuses() 将返回 op 对应的 tensor 对象，可以进一步获取 tensor 的 name, shape 等信息
for op in tf.get_default_graph().get_operations():
    print(op.name)
    print(op.values())

# Variables
# 显示默认计算图中所有的 variable（）对象
for variable in tf.all_variables():
    print(variable)
    print(variable.name)
```

## MKLDNN

```bash
export MKLDNN_VERBOSE=1
export MKL_VERBOSE=1
```
