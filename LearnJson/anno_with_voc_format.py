#!/usr/bin/python

import xml.etree.ElementTree as ET
import pickle
import sys, os, shutil
import argparse

import numpy as np
import cv2 as cv

# UI Image Size
IMAGE_WIDTH = 640
IMAGE_HEIGHT = 480


######## Xml Annotation Helper Functions #########
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
    
    
######## Annotation Helper Functions ##########
drawing=False
minx,miny=10,10
curx,cury=50,50
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
# Labeling UI
def getObject(image):
    global minx,miny,curx,cury,IMAGE_WIDTH,IMAGE_HEIGHT
    cv.namedWindow("Image",cv.WINDOW_AUTOSIZE)
    cv.setMouseCallback("Image",drawRectangle)
    while(1):
        # Resize Image into UI Coordinate
        imgtmp = cv.resize(image, (IMAGE_WIDTH, IMAGE_HEIGHT), 0, 0, cv.INTER_NEAREST)
        cv.rectangle(imgtmp,(minx,miny),(curx,cury),(0,255,0),1)
        cv.imshow("Image",imgtmp)
        # Get Label
        keyval = cv.waitKey(1)&0xFF
        if keyval == ord('c'):
            return ["c",minx,miny,curx,cury]
        elif keyval == ord('b'):
            return ["b",minx,miny,curx,cury] 
        elif keyval == ord('d'):
            return ["defect",minx,miny,curx,cury] 
        elif keyval == ord('w'):
            return ["w",minx,miny,curx,cury] 
        elif keyval == ord('e'):
            return ["e",minx,miny,curx,cury] 
        elif keyval == ord('x'):
            return ["x",minx,miny,curx,cury]
        elif keyval == ord('f'):
            return ["f",minx,miny,curx,cury]
        elif keyval == ord('r'):
            return ["r",minx,miny,curx,cury]
        elif keyval == 32: # SPACE
            return 0 # save and goto next image
        elif keyval == 8: # BACKSPACE
            return -1 # discard and goto next image
        elif keyval == 27: # ESC
            cv.destroyAllWindows()
            return -2 # exit
            
def getAnnoFromImage(tree,image):
    global minx,miny,curx,cury,IMAGE_WIDTH,IMAGE_HEIGHT
    num = 0
    while(1):
        # Show resized image
        image_resized = cv.resize(image, (416,416), 0, 0, cv.INTER_LINEAR)
        cv.imshow('resize', image_resized)
        # Get object from orginal image
        anobject = getObject(image)
        if anobject == -2: # exit
           return -2 
        elif anobject == -1: # discard and goto next image
            return -1
        elif anobject == 0: # save and goto next image
            return num
        else :
            # Transform bbox size from UI Coordinate to Actual Coordinate
            anobject[1] = int(anobject[1]*image.shape[1]/IMAGE_WIDTH)
            anobject[2] = int(anobject[2]*image.shape[0]/IMAGE_HEIGHT)
            anobject[3] = int(anobject[3]*image.shape[1]/IMAGE_WIDTH)
            anobject[4] = int(anobject[4]*image.shape[0]/IMAGE_HEIGHT)
            # Append objects into xml tree
            addObject(tree,anobject)
            num=num+1
            cv.rectangle(image,(anobject[1],anobject[2]),(anobject[3],anobject[4]),(0,0,255),9)
            cv.putText(image,str(anobject[0]),(anobject[1],anobject[2]),font,2,(0,0,255),4)
            #
            minx,miny,curx,cury=10,10,50,50

# parse console arguments
def get_parser():
    parser = argparse.ArgumentParser(description='annotate images in voc format.')
    parser.add_argument('--path',dest='dir_path',help='directory contains original images')
    return parser


if __name__ == "__main__":

    # Parser the Arguments
    parser = get_parser()
    args = parser.parse_args()
    dir_path = args.dir_path
    if not dir_path or not os.path.exists(dir_path):
        parser.print_help()
        sys.exit(-1)

    # Check the destination directories
    if not os.path.exists('VOC/'):
        os.makedirs('VOC/VOC2016/Annotations/')
        os.makedirs('VOC/VOC2016/ImageSets/Main/')
        os.makedirs('VOC/VOC2016/JPEGImages/')
    if not os.path.exists('VOC/VOC2016/Annotations/'):
        os.makedirs('VOC/VOC2016/Annotations/')
    if not os.path.exists('VOC/VOC2016/ImageSets/Main/'):
        os.makedirs('VOC/VOC2016/ImageSets/Main/')
    if not os.path.exists('VOC/VOC2016/JPEGImages/'):
        os.makedirs('VOC/VOC2016/JPEGImages/')

    # Traverse all images
    for file in os.listdir(dir_path):
        imagename = file
        # Create xml tree
        xmltree = createXMLTree(imagename)
        image = cv.imread(os.path.join(dir_path,file),cv.IMREAD_COLOR)
        imagesize = image.shape
        # Update Image Size Info
        addImageSize(xmltree,imagesize)
        # Get labeled objects of current image
        nums= getAnnoFromImage(xmltree,image)
        
        # Save xml or not
        if nums > 0:
            xmltree.write('VOC/VOC2016/Annotations/'+imagename.split('.')[0]+'.xml');
            shutil.move(os.path.join(dir_path,file),'VOC/VOC2016/JPEGImages/'+file)
            listFile = open('VOC/VOC2016/ImageSets/Main/'+'train.txt','a+')
            listFile.write(imagename.split('.')[0]+'\n')
            listFile.close()
        elif nums == -2:
            break
    sys.exit(0)

