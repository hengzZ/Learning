# coding: utf-8
import os
import sys
import argparse

import numpy as np
from PIL import Image

import cv2
import caffe


PROTOTXT = 'deploy.prototxt'
CAFFEMODEL = 'snapshot/train_iter_80000.caffemodel'


def init_net(prototxt, caffemodel):
    """
    Initialize a network from prototxt and caffemodel
    """
    net = caffe.Net(prototxt, caffemodel, caffe.TEST)
    return net


def predict_cv_im(net, cv_im):
    org_height = cv_im.shape[0]
    org_width = cv_im.shape[1]
    #im = cv2.resize(cv_im, (640, 480), interpolation=cv2.INTER_LINEAR)
    im = cv2.resize(cv_im, (800, 800), interpolation=cv2.INTER_LINEAR)
    #im = Image.fromarray(im_temp)
    #im = im.resize((416,416),Image.BILINEAR)
    in_ = np.array(im, dtype=np.float32)
    in_ = in_[np.newaxis, ...]
    in_ = in_ / [255.]
    print(in_.shape)

    net.blobs['data'].reshape(1, *in_.shape)
    net.blobs['data'].data[...] = in_
    # run net and take argmax for prediction
    net.forward()
    out = net.blobs['score'].data[0].argmax(axis=0)

    arr = out.astype(np.uint8)
    im = Image.fromarray(arr)

    palette=[]
    for i in range(256):
        palette.extend((i,i,i))
    palette[:3*21]=np.array([[0, 0, 0],
                                [128, 0, 0],
                                [0, 128, 0],
                                [128, 128, 0],
                                [0, 0, 128],
                                [128, 0, 128],
                                [0, 128, 128],
                                [128, 128, 128],
                                [64, 0, 0],
                                [192, 0, 0],
                                [64, 128, 0],
                                [192, 128, 0],
                                [64, 0, 128],
                                [192, 0, 128],
                                [64, 128, 128],
                                [192, 128, 128],
                                [0, 64, 0],
                                [128, 64, 0],
                                [0, 192, 0],
                                [128, 192, 0],
                                [0, 64, 128]], dtype='uint8').flatten()

    im.putpalette(palette)
    im = im.resize((org_width,org_height), Image.BILINEAR)
    return im


def get_parser():
    """
    Create a parser to parse arguments
    """
    parser = argparse.ArgumentParser('Do segmentation inference')
    parser.add_argument('--folder', dest='folder_path', help='a folder contains images')
    parser.add_argument('--video', dest='video', help='a video file')
    parser.add_argument('--image', dest='image', help='a image file')
    return parser


def main():
    parser = get_parser()
    args = parser.parse_args()
    net = init_net(PROTOTXT, CAFFEMODEL)

    if args.folder_path:
        folder_path = args.folder_path
        for image in os.listdir(folder_path):
            image_path = os.path.join(folder_path, image)
            org_im = cv2.imread(image_path, 0)
            im = predict_cv_im(net, org_im)
            im = im.convert("RGB")
            cv_im = np.asarray(im)[:,:,::-1].copy()
            #result = cv2.addWeighted(cv_im, 0.5, org_im, 0.5, 0)
            resized_im = cv2.resize(cv_im, (640, 480))
            cv2.imshow('demo', resized_im)
            if cv2.waitKey(0) & 0xFF == 27:
                break
        cv2.destroyAllWindows()
    elif args.video:
        video_path = args.video
        cap = cv2.VideoCapture(video_path)
        while(True):
            ret, frame = cap.read()
            if ret == False:
                print 'reach the end.'
                break
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            im = predict_cv_im(net, frame)
            im = im.convert("RGB")
            cv_im = np.asarray(im)[:,:,::-1].copy()
            result = cv2.addWeighted(cv_im, 0.5, frame, 0.5, 0)
            resized_im = cv2.resize(result, (640, 480))
            cv2.imshow('demo', resized_im)
            if cv2.waitKey(0) & 0xFF == 27:
                break
        cap.release()
        cv2.destroyAllWindows()
    elif args.image:
        image_path = args.image
        org_im = cv2.imread(image_path, 0)
        im = predict_cv_im(net, org_im)
        im = im.convert("RGB")
        cv_im = np.asarray(im)[:,:,::-1].copy()
        #result = cv2.addWeighted(cv_im, 0.5, org_im, 0.5, 0)
        resized_im = cv2.resize(cv_im, (640, 480))
        cv2.imshow('demo', resized_im)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    else:
        parser.print_help()


if __name__ == '__main__':
    sys.exit(main())

