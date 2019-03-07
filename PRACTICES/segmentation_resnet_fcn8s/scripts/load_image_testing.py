import numpy as np
from PIL import Image
import cv2


if __name__ == '__main__':
    im = Image.open('dataset/defect/JPEGImages/cls1_1.bmp')
    in_ = np.array(im, dtype=np.float32)
    print(in_)
    print(in_.shape)

    mask = Image.open('dataset/defect/SegmentationClass/cls1_1.png')
    label = np.array(mask, dtype=np.uint8)
    label = label[np.newaxis, ...]
    print(label)
    print(label.shape)

    clr_im = cv2.imread('dataset/defect/JPEGImages/cls1_1.bmp')
    #cv2.imwrite('gray2clr.jpg', clr_im)
    #clr_im = Image.open('gray2clr.jpg')
    clr_in_ = np.array(clr_im, dtype=np.float32)
    #clr_in_ = clr_in_[:,:,::-1]  # RGB -> BGR
    clr_in_ = clr_in_/255.
    clr_in_ = clr_in_.transpose((2,0,1))  # HWC -> CHW
    print(clr_in_)
    print(clr_in_.shape)
