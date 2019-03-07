import caffe
import surgery, score

import numpy as np
import os
import sys

try:
    import setproctitle
    setproctitle.setproctitle(os.path.basename(os.getcwd()))
except:
    pass

weights = '../base-resnet-101.caffemodel'

# init
caffe.set_device(int(0))  # gpu 0
caffe.set_mode_gpu()

solver = caffe.SGDSolver('solver.prototxt')
solver.net.copy_from(weights)

## surgeries
#interp_layers = [k for k in solver.net.params.keys() if 'up' in k]
#surgery.interp(solver.net, interp_layers)

# scoring
val = np.loadtxt('../dataset/defect/ImageSets/Segmentation/val.txt', dtype=str)

for _ in range(25):
    solver.step(1000)
    score.seg_tests(solver, False, val, layer='score')
