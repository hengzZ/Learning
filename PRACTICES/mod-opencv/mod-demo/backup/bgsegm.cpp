/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
    > Created Time: 2017年02月13日 星期一 09时36分52秒
 ************************************************************************/

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
    int niters = 3;
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    Mat temp;
    
    dilate(mask, temp, Mat(), Point(-1,-1), niters);
    erode(temp, temp, Mat(), Point(-1,-1), niters*2);
    dilate(temp, temp, Mat(), Point(-1,-1), niters);       
    
    findContours( temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE  );
    
    dst = Mat::zeros(img.size(), CV_8UC3);
    
    if( contours.size() == 0  )
        return;
    
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0, largestComp = 0;
    double maxArea = 0;
    
    for( ; idx >= 0; idx = hierarchy[idx][0]  )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > maxArea  )
        {
            maxArea = area;
            largestComp = idx;
        }
    }
    Scalar color( 0, 0, 255  );
    drawContours( dst, contours, largestComp, color, FILLED, LINE_8, hierarchy  );
}

int main(int argc, char** argv)
{
    VideoCapture cap;
    bool update_bg_model = true;

    VideoCapture capt("../demo.mp4");
    if(!capt.isOpened()){
        std::cout << "Could not open video." << std::endl;
        return -1;
    }
    
    Mat tmp_frame, bgmask, out_frame;

    capt >> tmp_frame;
    if(tmp_frame.empty()){
        std::cout << "Could not read data from the video source." << std::endl;
        return -1;
    }

    namedWindow("video",1);
    namedWindow("segmented",1);

    Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(10);

    std::cout << "Loop." << std::endl;

    for(;;)
    {
        capt >> tmp_frame;
        if(tmp_frame.empty())
            break;
        bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
        refineSegments(tmp_frame, bgmask, out_frame);
        imshow("video",tmp_frame);
        imshow("segmented",out_frame);

        char keycode = (char)waitKey(30);
        if(keycode == 27)
            break;
        if(keycode == ' '){
            update_bg_model = !update_bg_model;
            std::cout << "Learn background is in state = " << update_bg_model << std::endl;
        }
    }
    return 0;
}
