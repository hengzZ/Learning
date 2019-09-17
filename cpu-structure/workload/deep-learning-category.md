## Deep Learning Category

* Inception V3 -- video analysis
* ResNet v2 -- tf2caffe

#### Introduction

##### Table of Workload Type - Sessions
* ResNet v2 - 1st version
* ResNet v2 - separate version
    * 1
    * 2
    * 3
    * 4

#### Installation

##### 1. MKLDNN

###### Note: ``cd scripts && ./prepare_mkl.sh`` for mkldnn dependency libraries.

##### cblas
```
cblas is included in external dependencies.
```
##### Make
```
cd build && cmake ..&& make
```

#### 2. OMP
* 实例扩展性的问题
##### KMP 设置
```bash
export KMP_AFFINITY="granularity=fine,noverbose"
```
具体影响，请参考： <br>
Using KMP_AFFINITY to create OpenMP* thread mapping to OS proc IDs
—— https://software.intel.com/en-us/articles/using-kmp-affinity-to-create-openmp-thread-mapping-to-os-proc-ids
