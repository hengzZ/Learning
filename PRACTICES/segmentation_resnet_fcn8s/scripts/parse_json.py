# coding: utf-8
import os
import sys

import json
import numpy as np
import cv2
import argparse

from PIL import Image
from PIL import ImageDraw

LABEL_IMAGE_EXT = '.png'
PALETTE = {'background': (0, 0, 0),
           'defect': (128, 0, 0),
           'cls02': (0, 128, 0),
           'cls03': (128, 128, 0),
           'cls04': (0, 0, 128),
           'cls05': (128, 0, 128),
           'cls06': (0, 128, 128),
           'cls07': (128, 128, 128),
           'cls08': (64, 0, 0),
           'cls09': (192, 0, 0),
           'cls10': (64, 128, 0),
           'cls11': (192, 128, 0),
           'cls12': (64, 0, 128),
           'cls13': (192, 0, 128),
           'cls14': (64, 128, 128),
           'cls15': (192, 128, 128),
           'cls16': (0, 64, 0),
           'cls17': (128, 64, 0),
           'cls18': (0, 192, 0),
           'cls19': (128, 192, 0),
           'cls20': (0, 64, 128)}


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


def get_image_size(dict):
    """
    return [width, height]
    """
    height = dict['imageHeight']
    width = dict['imageWidth']
    return [width, height]


def objects(dict):
    """
    yield label, polygon
    """
    for obj in dict['shapes']:
        label = obj['label']  # defect
        polygon = obj['points']
        shape_type = obj['shape_type']  # polygon
        yield label, polygon


def paint_object(image, label, polygon):
    pts = np.array(polygon, np.int32)
    pts = pts.reshape((-1, 1, 2))
    color = (PALETTE[label][2], PALETTE[label][1], PALETTE[label][0])
    cv2.fillPoly(image, [pts], color)


def get_parser():
    """
    create a parser to parse arguments 'json_folder', 'output_folder', 'label_image_type'
    """
    parser = argparse.ArgumentParser(description="convert json to label image")
    parser.add_argument("--json_folder", dest="json_folder", default='labels',
                        help="specify the label(json) folder. (default: labels)")
    parser.add_argument("--output_folder", dest="output_folder", default='output',
                        help="specify the output folder. (default: output)")
    parser.add_argument("--label_image_type", dest="label_image_type", default='int',
                        help="label image type: int or rgb.(default: int)")
    return parser


def convert_json_to_rgb(json_folder, output_folder):
    for file_name in os.listdir(json_folder):
        json_file = os.path.join(json_folder, file_name)
        label_image_file = os.path.join(output_folder, file_name.split('.')[0] + LABEL_IMAGE_EXT)

        dict = {}
        with open(json_file, 'r', encoding='utf-8') as f:
            dict = json.load(f)
        image_size = get_image_size(dict)
        label_image = np.zeros((image_size[1], image_size[0], 3), np.uint8)
        for label, polygon in objects(dict):
            paint_object(label_image, label, polygon)

        cv2.imwrite(label_image_file, label_image)


def color_to_int(label_image, palette):
    palette_image = label_image.quantize(palette=palette)
    return palette_image


def convert_json_to_int(json_folder, output_folder):
    palette = create_palette_image()
    for file_name in os.listdir(json_folder):
        json_file = os.path.join(json_folder, file_name)
        label_image_file = os.path.join(output_folder, file_name.split('.')[0] + LABEL_IMAGE_EXT)

        dict = {}
        with open(json_file, 'r', encoding='utf-8') as f:
            dict = json.load(f)
        image_size = get_image_size(dict)
        label_image = np.zeros((image_size[1], image_size[0], 3), np.uint8)
        for label, polygon in objects(dict):
            paint_object(label_image, label, polygon)

        # BGR2RGB (opencv to pillow)
        label_image = Image.fromarray(cv2.cvtColor(label_image, cv2.COLOR_BGR2RGB))
        # color2int
        int_label = color_to_int(label_image, palette)

        int_label.save(label_image_file)


if __name__ == '__main__':
    parser = get_parser()
    args = parser.parse_args()  # parser.print_help()
    json_folder = str(args.json_folder)
    output_folder = str(args.output_folder)
    label_image_type = str(args.label_image_type)
    print("json_folder: ", json_folder, "\noutput_folder: ", output_folder, "\nlabel_image_type: ", label_image_type)

    if not os.path.exists(json_folder):
        print("\n{0} doesn't exist!!\n".format(json_folder))
        parser.print_help()
        sys.exit(0)
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    if 'int' == label_image_type:
        convert_json_to_int(json_folder, output_folder)
    elif 'rgb' == label_image_type:
        convert_json_to_rgb(json_folder, output_folder)
