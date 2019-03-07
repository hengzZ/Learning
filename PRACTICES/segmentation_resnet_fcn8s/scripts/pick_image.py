# coding: utf-8
import os
import sys
import argparse
import shutil

import numpy as np
from PIL import Image
import cv2


def get_parser():
    """
    create a parser to parse arguments 'image_folder', 'create_bg_mask'
    """
    parser = argparse.ArgumentParser(description="pick image")
    parser.add_argument("--image_folder", dest="image_folder", default='images',
                        help="specify the image folder. (default: images)")
    parser.add_argument("--create_bg_mask", dest="create_bg_mask", default=True,
                        help="create background mask.(default: True)")
    return parser


def create_palette_image():
    image_data = np.ones((224, 224), np.uint8)
    palette_image = Image.fromarray(image_data)

    palette = []
    for i in range(256):
        palette.extend((i, i, i))
    palette[:3 * 21] = np.array([[0, 0, 0],
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
    palette_image.putpalette(palette)
    return palette_image


def color_to_int(label_image, palette):
    palette_image = label_image.quantize(palette=palette)
    return palette_image


def create_mask(image_file):
    palette = create_palette_image()
    im = cv2.imread(image_file)
    print(im.shape)
    height = im.shape[0]
    width = im.shape[1]

    label_image = np.zeros((height, width, 3), np.uint8)

    # BGR2RGB (opencv to pillow)
    label_image = Image.fromarray(cv2.cvtColor(label_image, cv2.COLOR_BGR2RGB))
    # color2int
    int_label = color_to_int(label_image, palette)
    
    # int_label.save(label_image_file)
    return int_label  


def pick_images(image_folder, create_bg_mask):
    for image in os.listdir(image_folder):
        image_file = os.path.join(image_folder, image)
        org_im = cv2.imread(image_file)

        resized_im = cv2.resize(org_im, (640, 480))

        cv2.imshow('review', resized_im)
        keyval = cv2.waitKey(0)
        if keyval == ord('s'):
            # create background mask
            if create_bg_mask:
                mask_dir = os.path.join(image_folder, '../picked_masks')
                if not os.path.exists(mask_dir):
                    os.makedirs(mask_dir)
                mask_image = create_mask(image_file)
                mask_file = os.path.join(mask_dir, image.split('.')[0]+'.png')
                mask_image.save(mask_file)
            # move to dest
            dest_dir = os.path.join(image_folder, '../picked_images')
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
            dest_file = os.path.join(dest_dir, image)
            shutil.move(image_file, dest_file)
        if keyval == 27: break
    cv2.destroyAllWindows()
    return 0


if __name__ == '__main__':
    parser = get_parser()
    args = parser.parse_args()  # parser.print_help()
    image_folder = str(args.image_folder)
    create_bg_mask = str(args.create_bg_mask)
    print("image_folder: ", image_folder, "\ncreate_bg_mask: ", create_bg_mask)

    if not os.path.exists(image_folder):
        print("please confirm {0} exist!!".format(image_folder))
        parser.print_help()
        sys.exit(0)

    pick_images(image_folder, create_bg_mask)
    sys.exit(0)

