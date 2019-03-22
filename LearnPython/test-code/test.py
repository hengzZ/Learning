import os
import sys

import numpy as np
from input_loader import load_one_batch_images


if __name__ == '__main__':
    batch_size = 4
    height = 299
    width = 299
    channels = 3

    image_folder = '/dev/shm/images'
    image_list = os.listdir(image_folder)
    image_num = len(image_list)
    images = np.zeros((batch_size, channels, height, width), dtype=np.float32)

    image_list.sort()
    #for loop in range(10000000):
    for loop in range(1):
        batch_list = []
        for bidx in range(batch_size):
            batch_list.append(os.path.join(image_folder, image_list[bidx%image_num]))
        load_one_batch_images(batch_list, images, batch_size, channels, height, width)
        # convert [NCHM] to [NHWC]
        imgs = images.transpose(0,2,3,1)
        print(batch_list[-1], imgs.shape, imgs)
