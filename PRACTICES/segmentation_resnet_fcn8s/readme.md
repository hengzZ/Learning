## ResNet + FCN for Semantic Segmentation

#### 1. fcn caffe environment installation
```
## First, Install Caffe
git clone https://github.com/BVLC/caffe.git
cd caffe
cp Makefile.config.example Makefile.config
## Second, Deploy FCN environment.
## https://github.com/hengzZ/Learning/tree/master/PRACTICES/segmentation_resnet_fcn8s/
```

#### 2. Model Modification
1. You can combine a basic network structure into the fcn, such as combine resnet-101 with fcn8s
2. Modify the ***input_layers.py*** to adapt your input image and label mask.



###### reference
https://github.com/hengzZ/fcn.berkeleyvision.org

