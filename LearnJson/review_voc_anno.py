#!/usr/bin/python

import os
import sys
import errno
import argparse
import shutil
import pickle
import xml.etree.ElementTree as ET

from os import getcwd
from os.path import join

import numpy as np
import cv2 as cv

#
dst_dir = "./VOC_FILTERED"
xmlextension = ".xml"
imgextension = ".bmp"


### Mkdirs Helper Functions ###
def my_makedirs(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

### Labeling Helper Functions ###
def createXMLTree(imagename):
    root = ET.Element('annotation')
    
    folder = ET.SubElement(root,'folder')
    filename = ET.SubElement(root,'filename')
    
    source = ET.SubElement(root,'source')
    srdatabase = ET.SubElement(source,'database')
    srannotation = ET.SubElement(source,'annotation')
    srimage = ET.SubElement(source,'image')
    
    segmented = ET.SubElement(root,'segmented')
    
    folder.text = "VOC"
    filename.text = imagename
    srdatabase.text = "VOC"
    srannotation.text = "VOC"
    srimage.text = "VOC"
    segmented.text = "0"
    
    tplttree = ET.ElementTree(root)
    return tplttree
    
def addImageSize(tree,imagesize):
    root = tree.getroot()
    size = ET.SubElement(root,'size')
    szwidth = ET.SubElement(size,'width')
    szheight = ET.SubElement(size,'height')
    szdepth = ET.SubElement(size,'depth')
    
    szheight.text = str(imagesize[0])
    szwidth.text = str(imagesize[1])
    szdepth.text = str(imagesize[2])

def addObject(tree,obj):
    root = tree.getroot()
    anobject = ET.SubElement(root,'object')
    obname = ET.SubElement(anobject,'name')
    obpose = ET.SubElement(anobject,'pose')
    obtruncated = ET.SubElement(anobject,'truncated')
    obdifficult = ET.SubElement(anobject,'difficult')
    obbndbox = ET.SubElement(anobject,'bndbox')
    
    boxxmin = ET.SubElement(obbndbox,'xmin')
    boxymin = ET.SubElement(obbndbox,'ymin')
    boxxmax = ET.SubElement(obbndbox,'xmax')
    boxymax = ET.SubElement(obbndbox,'ymax')

    obname.text = str(obj[0])
    obpose.text = "Unspecified"
    obtruncated.text = "0"
    obdifficult.text = "0"
    boxxmin.text = str(obj[1])
    boxymin.text = str(obj[2])
    boxxmax.text = str(obj[3])
    boxymax.text = str(obj[4])    

### Drawing Helper Functions ###
drawing=False
minx,miny=30,30
curx,cury=80,80
font=cv.FONT_HERSHEY_SIMPLEX
# Callback Founction for Drawing
def drawRectangle(event,x,y,flags,param):
    global drawing,minx,miny,curx,cury
    if event==cv.EVENT_LBUTTONDOWN:
        drawing=True
        minx,miny=x,y
        curx,cury=x,y
    elif event==cv.EVENT_MOUSEMOVE:
        if drawing==True:
            curx,cury=x,y
    elif event==cv.EVENT_LBUTTONUP:
        drawing=False
        
def getObject(image):
    global minx,miny,curx,cury
    cv.namedWindow("Image",cv.WINDOW_AUTOSIZE)
    cv.setMouseCallback("Image",drawRectangle)
    while(1):
        imgtmp = image.copy()
        cv.rectangle(imgtmp,(minx,miny),(curx,cury),(0,255,0),1)
        cv.imshow("Image",imgtmp)
        # Labeling
        keyval = cv.waitKey(1)&0xFF
        if keyval == ord('c'):
            return ("c",minx,miny,curx,cury)
        elif keyval == ord('b'):
            return ("b",minx,miny,curx,cury) 
        elif keyval == ord('t'):
            return ("t",minx,miny,curx,cury) 
        elif keyval == ord('p'):
            return ("p",minx,miny,curx,cury) 
        elif keyval == ord('l'):
            return ("l",minx,miny,curx,cury) 
        elif keyval == ord('r'):
            return ("r",minx,miny,curx,cury)
        elif keyval == ord('g'):
            return ("g",minx,miny,curx,cury)
        elif keyval == ord('4'):
            return ("4",minx,miny,curx,cury)
        elif keyval == ord('6'):
            return ("6",minx,miny,curx,cury)
        elif keyval == ord('8'):
            return ("8",minx,miny,curx,cury)
        elif keyval == 32: # SPACE
            return 0 # save and goto next image
        elif keyval == 8: # BACKSPACE
            return -1 # discard and goto next image
        elif keyval == 27: # ESC
            cv.destroyAllWindows()
            return -2 # exit
            
def getAnnoFromImage(tree,image):
    global minx,miny,curx,cury
    num = 0
    while(1):
        # Get object
        anobject = getObject(image)
        if anobject == -2: # exit
           return -2 
        elif anobject == -1: # discard and goto next image
            return -1
        elif anobject == 0: # save and goto next image
            return num
        else :
            # Append objects into xml anno
            addObject(tree,anobject)
            num=num+1
            cv.rectangle(image,(anobject[1],anobject[2]),(anobject[3],anobject[4]),(0,0,255),1)
            cv.putText(image,str(anobject[0]),(anobject[1],anobject[2]),font,1,(0,0,255),2)
            # default box 
            minx,miny,curx,cury=30,30,80,80

### Preview Helper Functions ###
def boxs(anno_file_path):
    tree = ET.parse(anno_file_path)
    root = tree.getroot()
    for obj in root.findall("object"):
        obj_name = str(obj.find("name").text)
        bndbox = obj.find("bndbox")
        obj_xmin = int(bndbox.find("xmin").text)
        obj_ymin = int(bndbox.find("ymin").text)
        obj_xmax = int(bndbox.find("xmax").text)
        obj_ymax = int(bndbox.find("ymax").text)
        yield [obj_name, obj_xmin, obj_ymin, obj_xmax, obj_ymax]

def draw_result(window_name,image_file_path,anno_file_path):
    image = cv.imread(image_file_path)
    special = True
    for box in boxs(anno_file_path):
        if "car" == box[0]:
            color = (0,0,255)
        elif "person" == box[0]:
            color = (0,255,0)
        elif "bus" == box[0]:
            color = (255,0,0)
        elif "cookie" == box[0]:
            color = (255,20,147)
        elif "bear" == box[0]:
            color = (255,0,0)
        elif "dog" == box[0]:
            color = (250,128,114)
        elif "wipe" == box[0]:
            color = (244,164,96)
        elif "noodle" == box[0]:
            color = (240,230,140)
        elif "milk" == box[0]:
            color = (240,128,128)
        elif "puffed" == box[0]:
            color = (238,130,238)
        elif "drink" == box[0]:
            color = (50,205,50)
        else:
            color = (0,255,0)
        font = cv.FONT_HERSHEY_COMPLEX
        cv.putText(image, box[0], (box[1],box[2]-5), font, 1.5, color, 1, cv.LINE_AA)
        cv.rectangle(image, (box[1],box[2]), (box[3],box[4]), color, 5)
        # Filter 
        if "red" == box[0] or "green" == box[0]:
            special = True
    # show result
    if special == False:
        return ord('1') # pass (don't show image)
    else:
        image_resized = cv.resize(image, (416,416), 0, 0, cv.INTER_LINEAR)
        cv.imshow('resize', image_resized)
        cv.imshow(window_name, image)
        cv.imwrite('demo.jpg', image)
        keyval = cv.waitKey(0)&0xFF
        return keyval

# preview
def preview(index_file,images_folder_path,annos_folder_path):
    with open(index_file) as file:
        window_name = "Image"
        cv.namedWindow(window_name,cv.WINDOW_AUTOSIZE)
        for line in file:
            image_file_path = join(images_folder_path, line.strip()+imgextension)
            anno_file_path = join(annos_folder_path, line.strip()+xmlextension)
            # display result
            ret = draw_result(window_name,image_file_path,anno_file_path)
            if 27 == ret:
                cv.destroyAllWindows()
                break
            elif 32 == ret: # Space
                # Move image and copy annotation file
                shutil.copy(anno_file_path, join(dst_dir, "Annotations/"+line.strip()+xmlextension))
                shutil.move(image_file_path, join(dst_dir, "JPEGImages/"+line.strip()+imgextension)) 
            elif ord('d') == ret:
                # New labeling
                image_name = line.strip()+imgextension
                xml_tree = createXMLTree(image_name)
                img_cv = cv.imread(image_file_path)
                img_size = img_cv.shape
                addImageSize(xml_tree, img_size)
                # Labeling
                nums = getAnnoFromImage(xml_tree, img_cv)
                # Save or not
                if nums > 0:
                    xml_tree.write(join(dst_dir, "Annotations/"+line.strip()+xmlextension))
                    shutil.move(image_file_path, join(dst_dir, "JPEGImages/"+line.strip()+imgextension)) 
                elif nums == -2:
                    break


# Argument Parsing Helper
def get_parser():
    parser = argparse.ArgumentParser('review labeled voc dataset')
    parser.add_argument('--path', dest='dataset', help='dataset path such as: [VOCdevkit]/VOC2007/')
    return parser


if __name__ == "__main__":

    # Parse the Arguments
    parser = get_parser()
    args = parser.parse_args()
    dataset = args.dataset
    if not dataset or not os.path.exists(dataset):
        parser.print_help()
        sys.exit(-1)

    # Check the VOC Folder Tree
    if not os.path.exists(join(dataset,"Annotations")):
        print("directory %s not exist" % (join(dataset,"Annotations")))
        sys.exit(-1)
    if not os.path.exists(join(dataset,"ImageSets/Main")):
        print("directory %s not exist" % (join(dataset,"ImageSets/Main")))
        sys.exit(-1)
    if not os.path.exists(join(dataset,"JPEGImages")):
        print("directory %s not exist" % (join(dataset,"JPEGImages")))
        sys.exit(-1)
    # Make Destination Folders (make -p)
    my_makedirs(join(dst_dir, "Annotations"))
    my_makedirs(join(dst_dir, "JPEGImages"))
    my_makedirs(join(dst_dir, "ImageSets/Main"))

    # Get Image Index File
    imagesets_dir = join(dataset,"ImageSets/Main")
    print("Directory %s contains %s" % (imagesets_dir, os.listdir(imagesets_dir)))
    idx_file_name = input("Please enter the file name for preview: \n$ ")
    print("You Selected %s for preview" % (join(imagesets_dir,idx_file_name)))

    # Preview ImageSets
    preview(join(imagesets_dir,idx_file_name),join(dataset,"JPEGImages"),join(dataset,"Annotations"))

    sys.exit(0)

