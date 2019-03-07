#!/usr/bin/python

import os
import sys

import numpy as np
import caffe


prototxt = "resnet-101/models/pre_trained_models/ResNet-101L/ResNet-101-deploy.prototxt"
weights = "resnet-101/models/pre_trained_models/ResNet-101L/ResNet-101-model.caffemodel"

def copy(org_net, dest_net):
    params_keys = dest_net.params.keys()
    for pr in params_keys:
	lidx = list(dest_net._layer_names).index(pr)
	oidx = list(org_net._layer_names).index(pr)
	layer = dest_net.layers[lidx]

	if layer.type == "Convolution" and len(dest_net.params[pr]) == 2:
	    dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
	    dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
	elif layer.type == "Convolution" and len(dest_net.params[pr]) == 1:
	    dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
	elif layer.type == 'BatchNorm':
	    dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
	    dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
	    dest_net.layers[lidx].blobs[2].data.flat = org_net.layers[oidx].blobs[2].data.flat
	elif layer.type == 'Scale':
	    dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
	    dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
	else:
	    if len(dest_net.params[pr]) > 0:
		print 'missing !!!', layer.type


def main():
    # init org net
    net = caffe.Net(prototxt, weights, caffe.TEST)
    # init cut net
    dest_net = caffe.Net("base-resnet-101.prototxt",caffe.TEST)
    # weights assignment
    copy(net, dest_net)
    # export caffemodel
    dest_net.save("base-resnet-101.caffemodel")
    return 0


if __name__ == "__main__":
    sys.exit(main())
