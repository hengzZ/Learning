# coding: utf-8
import os
import sys
import argparse

import numpy as np
from PIL import Image
import cv2


def get_parser():
    """
    create a parser to parse arguments 'image_folder', 'mask_folder', 'review_type'
    """
    parser = argparse.ArgumentParser(description="review label/mask")
    parser.add_argument("--image_folder", dest="image_folder", default='images',
                        help="specify the image folder. (default: images)")
    parser.add_argument("--mask_folder", dest="mask_folder", default='masks',
                        help="specify the mask folder. (default: masks)")
    parser.add_argument("--review_type", dest="review_type", default='mask',
                        help="review type: mask or pixel.(default: mask)")
    return parser


def review_mask(image_folder, mask_foler):
    for image in os.listdir(image_folder):
        image_file = os.path.join(image_folder, image)
        mask_file = os.path.join(mask_folder, image.split('.')[0] + '.png')
        org_im = cv2.imread(image_file)
        mask_im = cv2.imread(mask_file)
        mixed_im = cv2.addWeighted(mask_im, 0.5, org_im, 0.5, 0)

        cv2.imwrite('temp.jpg', mixed_im)
        cv2.imshow('review', mixed_im)
        keyval = cv2.waitKey(0)
        if keyval == 27: break
    cv2.destroyAllWindows()
    return 0


def review_pixel(image_folder, mask_foler):
    for mask in os.listdir(mask_foler):
        mask_file = os.path.join(mask_foler, mask)
        labels = Image.open(mask_file)
        width, height = labels.size
        fg_val = 0
        for x in range(0, width):
            for y in range(0, height):
                val = labels.getpixel((x, y))
                if 0 != int(val):
                    fg_val = val
                    break
            if 0 != fg_val:
                break
        print('first fg label: ', fg_val)
    return 0

if __name__ == '__main__':
    parser = get_parser()
    args = parser.parse_args()  # parser.print_help()
    image_folder = str(args.image_folder)
    mask_folder = str(args.mask_folder)
    review_type = str(args.review_type)
    print("image_folder: ", image_folder, "\nmask_folder: ", mask_folder, "\nreview_type: ", review_type)

    if not os.path.exists(image_folder) or not os.path.exists(mask_folder):
        print("please confirm {0},{1} exist!!")
        parser.print_help()
        sys.exit(0)

    if 'mask' == review_type:
        review_mask(image_folder, mask_folder)
    elif 'pixel' == review_type:
        review_pixel(image_folder, mask_folder)
    sys.exit(0)
