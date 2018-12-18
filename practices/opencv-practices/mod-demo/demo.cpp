/*************************************************************************
	> File Name: demo.cpp
	> Author: zhihengw
	> Mail: 
	> Created Time: 2017年02月15日 星期三 12时54分30秒
 ************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videostab.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::videostab;


int temporaldiff(void)
{
    VideoCapture capt("./demo480.mp4");
    if(!capt.isOpened()){
        cout << "Could not open the capture." << endl;
        return -1;
    }

    namedWindow("framediff",WINDOW_AUTOSIZE);

    // Define the Mats for buffer
    Mat fs, frame, preframe, difframe, diffgray;

    while(true)
    {
        capt >> fs;
        if(fs.empty()) break;
        // Resize the :frame 
        double fy = (double)240/fs.rows;
        resize(fs,frame,Size(0,0),fy,fy);

		// Interframe difference
		if(preframe.empty()){
			frame.copyTo(preframe);
		}
		absdiff(preframe, frame, difframe);
		frame.copyTo(preframe);
		
        //imshow("bgsegm", bgmask);
        cvtColor(difframe,diffgray,COLOR_BGR2GRAY);
		imshow("framediff", diffgray);
        
		char key = (char)waitKey(15);
        if(27 == key) break;
    }

    destroyAllWindows();
    return 0;
}


int bgsegm(void)
{
    VideoCapture capt("./demo480.mp4");
    // VideoCapture capt("./stabilized.avi");
    // VideoCapture capt(0);
    if(!capt.isOpened()){
        cout << "Could not open the capture." << endl;
        return -1;
    }

    // Define the display windows
    namedWindow("bgsegm",WINDOW_AUTOSIZE);

    // Define the Mats for buffer
    Mat frame, preframe, bgmask, bgmaskknn;

    // Define background subtractor
    Ptr<BackgroundSubtractorMOG2> bgsubtractorMOG2 = createBackgroundSubtractorMOG2();
    bgsubtractorMOG2->setHistory(20);
    // bgsubtractorMOG2->setVarThreshold(15);
    bgsubtractorMOG2->setDetectShadows(false);
    // Ptr<BackgroundSubtractorKNN> bgsubtractorKNN = createBackgroundSubtractorKNN();
    // Ptr<bgsegm::BackgroundSubtractorGMG> bgsubtractorGMG = bgsegm::createBackgroundSubtractorGMG();
    // Ptr<bgsegm::BackgroundSubtractorMOG> bgsubtractorMOG = bgsegm::createBackgroundSubtractorMOG();

    while(true)
    {
        capt >> preframe;

        if(preframe.empty()) break;
        // Background Segmentation
		bgsubtractorMOG2->apply(preframe, bgmask, -1); // 0-not update 1-reinitialized -1 - automatically chosen 
		// bgsubtractorMOG2->apply(frame, bgmask, 0.9);  
		// bgsubtractorKNN->apply(frame, bgmaskknn);	
		
        imshow("bgsegm", bgmask);
        
		char key = (char)waitKey(15);
        if(27 == key) break;
    }

    destroyAllWindows();
    return 0;
}


// OpticalFlow measurement
inline bool isFlowCorrect(Point2f u)
{
    return !cvIsNaN(u.x) && !cvIsNaN(u.y) && fabs(u.x) < 1e9 && fabs(u.y) < 1e9;
}

static Vec3b computeColor(float fx, float fy)
{
    static bool first = true;

    // relative lengths of color transitions:
    // these are chosen based on perceptual similarity
    // (e.g. one can distinguish more shades between red and yellow
    //  than between yellow and green)
    const int RY = 15;
    const int YG = 6;
    const int GC = 4;
    const int CB = 11;
    const int BM = 13;
    const int MR = 6;
    const int NCOLS = RY + YG + GC + CB + BM + MR;
    static Vec3i colorWheel[NCOLS];

    if (first)
    {
        int k = 0;

        for (int i = 0; i < RY; ++i, ++k)
            colorWheel[k] = Vec3i(255, 255 * i / RY, 0);

        for (int i = 0; i < YG; ++i, ++k)
            colorWheel[k] = Vec3i(255 - 255 * i / YG, 255, 0);

        for (int i = 0; i < GC; ++i, ++k)
            colorWheel[k] = Vec3i(0, 255, 255 * i / GC);

        for (int i = 0; i < CB; ++i, ++k)
            colorWheel[k] = Vec3i(0, 255 - 255 * i / CB, 255);

        for (int i = 0; i < BM; ++i, ++k)
            colorWheel[k] = Vec3i(255 * i / BM, 0, 255);

        for (int i = 0; i < MR; ++i, ++k)
            colorWheel[k] = Vec3i(255, 0, 255 - 255 * i / MR);

        first = false;
    }

    const float rad = sqrt(fx * fx + fy * fy);
    const float a = atan2(-fy, -fx) / (float)CV_PI;

    const float fk = (a + 1.0f) / 2.0f * (NCOLS - 1);
    const int k0 = static_cast<int>(fk);
    const int k1 = (k0 + 1) % NCOLS;
    const float f = fk - k0;

    Vec3b pix;

    for (int b = 0; b < 3; b++)
    {
        const float col0 = colorWheel[k0][b] / 255.f;
        const float col1 = colorWheel[k1][b] / 255.f;

        float col = (1 - f) * col0 + f * col1;

        if (rad <= 1)
            col = 1 - rad * (1 - col); // increase saturation with radius
        else
            col *= .75; // out of range

        pix[2 - b] = static_cast<uchar>(255.f * col);
    }

    return pix;
}

static void drawOpticalFlow(const Mat_<Point2f>& flow, Mat& dst, float maxmotion = -1)
{
    dst.create(flow.size(), CV_8UC3);
    dst.setTo(Scalar::all(0));

    // determine motion range:
    float maxrad = maxmotion;

    if (maxmotion <= 0)
    {
        maxrad = 1;
        for (int y = 0; y < flow.rows; ++y)
        {
            for (int x = 0; x < flow.cols; ++x)
            {
                Point2f u = flow(y, x);

                if (!isFlowCorrect(u))
                    continue;

                maxrad = max(maxrad, sqrt(u.x * u.x + u.y * u.y));
            }
        }
    }

    for (int y = 0; y < flow.rows; ++y)
    {
        for (int x = 0; x < flow.cols; ++x)
        {
            Point2f u = flow(y, x);

            if (isFlowCorrect(u))
                dst.at<Vec3b>(y, x) = computeColor(u.x / maxrad, u.y / maxrad);
        }
    }
}

int optflow(void)
{
    // Define the VideoCapture
    VideoCapture capt("./demo480.mp4");
    // VideoCapture capt(0);
    if(!capt.isOpened()){
        cout << "Could not open the capture." << endl;
        return -1;
    }
    // Define the display windows
    namedWindow("opticalflow", WINDOW_AUTOSIZE);

    // Define the Mats for buffers
    Mat preframe, frame;
    UMat pregray, crtgray;
    
    Mat_<Point2f> flow;
    Ptr<DualTVL1OpticalFlow> tvl1 = cv::DualTVL1OpticalFlow::create();

    while(true)
    {
        capt >> preframe;
        waitKey(30);
        capt >> frame;
        if(preframe.empty()) break;
        
        const double start = (double)getTickCount();
        // Convert color space
        if(preframe.size() != frame.size()){
            cout << "Images should be of equal sizes" << endl;
            break;
        }
        cvtColor(preframe, pregray, COLOR_BGR2GRAY);
        cvtColor(frame, crtgray, COLOR_BGR2GRAY);
        tvl1->calc(pregray, crtgray, flow);
        const double timeSec = (getTickCount() - start) / getTickFrequency();
        cout << "calcOpticalFlowDual_TVL1 : " << timeSec << " sec" << endl;

        Mat out;
        drawOpticalFlow(flow, out);
        imshow("opticalflow", out);
        // imshow("opticalflow",frame);
        char key = (char)waitKey(15);
        if(27 == key) break;
    }
    destroyAllWindows();
    return 0;
}



// Gunnar Farneback Optical Flow Algorithm
static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

int GFoptflow(void)
{
    VideoCapture cap("./demo480.mp4");
    // VideoCapture cap(0);
    if( !cap.isOpened() ) return -1;

    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;)
    {
        cap >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( !prevgray.empty() )
        {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
        }
        if(waitKey(15)>=0)
            break;
        std::swap(prevgray, gray);
    }
    return 0;
}


//// REFERENCE: opencv/samples/cpp/videostab.cpp
int framestab(void)
{
    StabilizerBase* stabilizer = 0;
    Ptr<VideoFileSource> source = makePtr<VideoFileSource>("./demo480.mp4");

    //ONE PASS 
    OnePassStabilizer* onePassStabilizer = new OnePassStabilizer();
    stabilizer = onePassStabilizer;
    //"stdev"=="auto"
    onePassStabilizer->setMotionFilter(makePtr<GaussianMotionFilter>(15));

    //Motion Estimator 
    Ptr<MotionEstimatorRansacL2> est = makePtr<MotionEstimatorRansacL2>(MM_TRANSLATION_AND_SCALE);
    RansacParams ransac = est->ransacParams();
    ransac.eps = 0.5;
    est->setRansacParams(ransac);
    est->setMinInlierRatio(0.1);
    Ptr<KeypointBasedMotionEstimator> kbest = makePtr<KeypointBasedMotionEstimator>(est);
    kbest->setDetector(GFTTDetector::create(1000));
    Ptr<IOutlierRejector> outlierRejector = makePtr<NullOutlierRejector>();
    kbest->setOutlierRejector(outlierRejector);

    //Stabilizer Setting
    stabilizer->setFrameSource(source);
    stabilizer->setMotionEstimator(kbest);

    stabilizer->setRadius(15);
    stabilizer->setTrimRatio(0.1);
    stabilizer->setCorrectionForInclusion(true);
    stabilizer->setBorderMode(BORDER_REFLECT);

    // Define the display windows
    // namedWindow("framediff",WINDOW_AUTOSIZE);
    namedWindow("bgsegm",WINDOW_AUTOSIZE);

    // Define the Mats for buffer
    Mat frame, preframe, bgmask, bgmaskknn, difframe, diffgray;

    // Define background subtractor
    Ptr<BackgroundSubtractorMOG2> bgsubtractorMOG2 = createBackgroundSubtractorMOG2();
    bgsubtractorMOG2->setHistory(20);
    // bgsubtractorMOG2->setVarThreshold(15);
    bgsubtractorMOG2->setDetectShadows(false);
    // Ptr<BackgroundSubtractorKNN> bgsubtractorKNN = createBackgroundSubtractorKNN();
    // Ptr<bgsegm::BackgroundSubtractorGMG> bgsubtractorGMG = bgsegm::createBackgroundSubtractorGMG();
    // Ptr<bgsegm::BackgroundSubtractorMOG> bgsubtractorMOG = bgsegm::createBackgroundSubtractorMOG();

    Ptr<IFrameSource> stabilizedFrames = dynamic_cast<IFrameSource*>(stabilizer);
    while(!(preframe= stabilizedFrames->nextFrame()).empty())
    {
		// if(frame.empty()){
			// preframe.copyTo(frame);
		// }
		// absdiff(preframe, frame, difframe);
		// preframe.copyTo(frame);
		
	    // cvtColor(difframe,diffgray,COLOR_BGR2GRAY);
		// imshow("framediff",diffgray);

	 	// Background Segmentation
		// bgsubtractorMOG2->apply(preframe, bgmask, -1); // 0-not update 1-reinitialized -1 - automatically chosen 
		// bgsubtractorMOG2->apply(frame, bgmask, 0.9);  
		// bgsubtractorKNN->apply(frame, bgmaskknn);	
        // imshow("bgsegm", bgmask);

        // imshow("stabilizedFrame",preframe);
        char key = static_cast<char>(waitKey(15));
        if(27 == key) {cout << endl; break;}
    }

	destroyAllWindows();
    return 0;
}


int main(int argc, char** argv)
{
    //int ret;
    //ret = temporaldiff();
    //ret = bgsegm();
    //ret = optflow();
    //ret = GFoptflow();
    //ret = framestab();

    //
    //VideoCapture capt("./demo480.mp4");
    VideoCapture capt("./mod-demo.avi");
    //VideoCapture capt(0);
    if(!capt.isOpened()){
        cout << "Could not open the video(camera)." << endl;
        return -1;
    }
    namedWindow("demo",WINDOW_AUTOSIZE);
    Mat premat, curmat, frame, rsframe, diffmat, diffgray, binmat, erosion, dilation;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);

    while(true)
    {
        capt >> frame;
        if(frame.empty()) break;
        
        //
        double fy = (double)240/frame.rows;
        resize(frame,rsframe,Size(0,0),fy,fy);
        medianBlur(rsframe, curmat, 3);
        if(premat.empty()) curmat.copyTo(premat);

        absdiff(premat,curmat,diffmat);
        cvtColor(diffmat,diffgray,COLOR_BGR2GRAY);

        threshold(diffgray,binmat,25,255,THRESH_BINARY_INV);
        //threshold(diffgray,binmat,5,255,THRESH_BINARY);

        erode(binmat,erosion,Mat());
        dilate(erosion,dilation,Mat());
        
        findContours(dilation, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        Mat drawing = Mat::zeros(dilation.size(), CV_8UC3);
        for(size_t i = 0; i< contours.size(); i++)
        {
            Scalar color = Scalar(255, 255, 255);
            //Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
            drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());     
        }
        //Eliminate border 
        //rectangle(drawing,Point(0,0),Point(drawing.cols-1,drawing.rows-1),Scalar(0,0,0),3,8);

        curmat.copyTo(premat);

        imshow("demo",drawing);
        imshow("video",frame);
        char key = (char)waitKey(30);
        if(27 == key) break;
    }

    destroyAllWindows();
    return 0;
}
