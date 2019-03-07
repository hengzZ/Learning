#!/usr/bin/python

import os
import sys
import argparse
import logging


logger = logging.getLogger()
logger.setLevel(logging.INFO)


def main(args):
    #print args.image_dir
    if not os.path.exists(args.image_dir):
        logging.info("Directory %s not exist." % args.image_dir)
        return 1
    with open('train.txt','w') as f:
        for file in os.listdir(args.image_dir):
            image_id = str(file).strip().split('.')[0]
            if '' == image_id or '\n' == image_id: continue
            f.write(image_id + '\n')
    return 0

def parse_args():
    parser = argparse.ArgumentParser(description='Create train.txt containing image ids.')
    parser.add_argument('--img-dir',
                        dest='image_dir',
                        help='directory contains training images')
    args = parser.parse_args()
    logging.info(args)
    return args


if __name__ == "__main__":
    args = parse_args()
    sys.exit(main(args))
