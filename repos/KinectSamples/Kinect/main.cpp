#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <Kinect.h>

using namespace std;
using namespace cv;


// Release Kinect Handle
template<class Interface>
void SafeRelease(Interface *& pInterfaceToRelease)
{
	if(pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

// Convert Depth to Mat 
Mat ConvertToMat(const UINT16* pBuffer, int nWidth, int nHeight)
{
	Mat img(nHeight, nWidth, CV_8UC1);
	uchar* p_mat = img.data;

	const UINT16* pBufferEnd = pBuffer + nWidth*nHeight;

	while(pBuffer < pBufferEnd){
		*p_mat++ = *pBuffer++ / 65536.0 * 256;
	}

	return img;
}


int main(int argc, char** argv)
{
	IKinectSensor* m_pKinectSensor = NULL;
	IColorFrameReader* m_pColorFrameReader;
	IDepthFrameReader* m_pDepthFrameReader;
	IInfraredFrameReader* m_pInfraredFrameReader;

	// Initialize the default Kinect sensor
	HRESULT hr;
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if(FAILED(hr)){
		cout << "No ready Kinect found!" << endl;
		return -1;
	}
	hr = m_pKinectSensor->Open();
	if(!m_pKinectSensor || FAILED(hr)){
		cout << "Kinect Open failed." << endl;
		return -1;
	}

	// Kinect Color Basics
	IColorFrameSource* pColorFrameSource = NULL;
	hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
	if(SUCCEEDED(hr)){
		hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
	}
	SafeRelease(pColorFrameSource);


	// Kinect Infrared Basics
	IInfraredFrameSource* pInfraredFrameSource = NULL;
	hr = m_pKinectSensor->get_InfraredFrameSource(&pInfraredFrameSource);
	if(SUCCEEDED(hr)){
		hr = pInfraredFrameSource->OpenReader(&m_pInfraredFrameReader);
	}
	SafeRelease(pInfraredFrameSource);


	// Kinect Depth Basics
	IDepthFrameSource* pDepthFrameSource = NULL;
	hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
	if(SUCCEEDED(hr)){
		hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
	}
	SafeRelease(pDepthFrameSource);

	int num = 1;
	Mat im_color, im_infrared, im_raw_depth, im_depth;
	VideoWriter writer_color, writer_infrared, writer_depth;
	
	while(1){

		// Color Frame
		if(!m_pColorFrameReader){
			cout << "No Color Frame Reader!" << endl;
			break;
		}
		IColorFrame* pColorFrame = NULL;
		hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);

		if(SUCCEEDED(hr)){
			INT64 nTime = 0;
			IFrameDescription* pFrameDescription = NULL;
			int nWidth = 0, nHeight = 0;
			ColorImageFormat imageFormat = ColorImageFormat_None;
			UINT nBufferSize = 0;
			RGBQUAD *pBuffer = NULL;
			uchar* pBuffer_color = NULL;

			hr = pColorFrame->get_RelativeTime(&nTime);
			if(SUCCEEDED(hr)){
				hr = pColorFrame->get_FrameDescription(&pFrameDescription);
			}

			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Width(&nWidth);
			}
			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Height(&nHeight);
			}

			if(SUCCEEDED(hr)){
				hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
			}

			if(SUCCEEDED(hr)){
				if(ColorImageFormat_Bgra == imageFormat){
					hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
				}
				else {
					im_color = Mat(nHeight, nWidth, CV_8UC4);
					pBuffer_color = im_color.data;
					nBufferSize = im_color.rows*im_color.step;
					hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer_color), ColorImageFormat_Bgra);
					imshow("im_color", im_color);
				}
			}

			SafeRelease(pFrameDescription);
		}
		SafeRelease(pColorFrame);


		// Infrared Frame
		if(!m_pInfraredFrameReader){
			cout << "No Infrared Frame Reader!" << endl;
			break;
		}
		IInfraredFrame* pInfraredFrame = NULL;
		hr = m_pInfraredFrameReader->AcquireLatestFrame(&pInfraredFrame);

		if(SUCCEEDED(hr)){
			INT64 nTime = 0;
			IFrameDescription* pFrameDescription = NULL;
			int nWidth = 0, nHeight = 0;
			UINT nBufferSize = 0;
			UINT16* pBuffer = NULL;

			hr = pInfraredFrame->get_RelativeTime(&nTime);
			if(SUCCEEDED(hr)){
				hr = pInfraredFrame->get_FrameDescription(&pFrameDescription);
			}

			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Width(&nWidth);
			}
			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Height(&nHeight);
			}

			if(SUCCEEDED(hr)){
				hr = pInfraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
			}
			// 
			//im_infrared = Mat(nHeight, nWidth, CV_16UC1);
			//pBuffer = (UINT16*)im_infrared.data;
			//nBufferSize = nHeight*nWidth;
			//pInfraredFrame->CopyFrameDataToArray(nBufferSize, pBuffer);
			im_infrared = ConvertToMat(pBuffer, nWidth, nHeight);
			imshow("im_infrared", im_infrared);
			
			SafeRelease(pFrameDescription);
		}
		SafeRelease(pInfraredFrame);


		// Depth Frame
		if(!m_pDepthFrameReader){
			cout << "No Depth Frame Reader!" << endl;
			break;
		}
		IDepthFrame* pDepthFrame = NULL;
		hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

		if(SUCCEEDED(hr)){
			INT64 nTime = 0;
			IFrameDescription* pFrameDescription = NULL;
			int nWidth = 0, nHeight = 0;
			USHORT nDepthMinReliableDistance = 0;
			USHORT nDepthMaxDistance = 0;
			UINT nBufferSize = 0;
			UINT16* pBuffer = NULL;

			hr = pDepthFrame->get_RelativeTime(&nTime);
			if(SUCCEEDED(hr)){
				hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
			}

			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Width(&nWidth);
			}
			if(SUCCEEDED(hr)){
				hr = pFrameDescription->get_Height(&nHeight);
			}

			if(SUCCEEDED(hr))
			{
				hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
			}
			if(SUCCEEDED(hr))
			{
				// In order to see the full range of depth (including the less reliable far field depth)
				// we are setting nDepthMaxDistance to the extreme potential depth threshold
				//nDepthMaxDistance = USHRT_MAX;

				// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
				hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
			}

			if(SUCCEEDED(hr)){
				hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
			}
			//			
			im_raw_depth = Mat(nHeight, nWidth, CV_16UC1);
			pBuffer = (UINT16*)im_raw_depth.data;
			nBufferSize = nHeight*nWidth;
			pDepthFrame->CopyFrameDataToArray(nBufferSize, pBuffer);
			imshow("im_raw_depth", im_raw_depth);
			im_depth = ConvertToMat(pBuffer, nWidth, nHeight);

			SafeRelease(pFrameDescription);
		}
		SafeRelease(pDepthFrame);


		// Video Writers
		if(writer_color.isOpened() && writer_depth.isOpened() && writer_infrared.isOpened()){
			writer_color.write(im_color);
			writer_depth.write(im_depth);
			writer_infrared.write(im_infrared);
		}

		char keyval = waitKey(15);
		if(27 == keyval) break;
		else if('s' == keyval){
			//int codec = CV_FOURCC('M','J','P','G');
			int codec = -1;
			double fps = 30.0;

			stringstream ss;
			string idx = "";
			ss << num; 
			ss >> idx;
			num++;

			string color_video = "color" + idx + ".mp4";
			string infrared_video = "infrared" + idx + ".mp4";
			string depth_video = "depth" + idx + ".mp4";
			writer_color.open(color_video,codec,fps,im_color.size(),1);
			writer_infrared.open(infrared_video,codec,fps,im_infrared.size(),0);
			writer_depth.open(depth_video,codec,fps,im_raw_depth.size(),0);
		}
		else if('e' == keyval){
			writer_color.release();
			writer_depth.release();
			writer_infrared.release();
		}
		else if(32 == keyval){
			stringstream ss;
			string idx = "";
			ss << num; 
			ss >> idx;
			num++;

			string im_color_file = "left_" + idx + ".jpg";
			string im_infrared_file = "right_" + idx + ".jpg";
			string im_raw_depth_file = "raw_depth_" + idx + ".xml";

			imwrite(im_color_file, im_color);
			imwrite(im_infrared_file, im_infrared);
			// Create a FileStorage to save Raw Depth data
			FileStorage fs(im_raw_depth_file, FileStorage::WRITE);
			fs << "Mat" << im_raw_depth;
			fs.release();
		}

	}

	destroyAllWindows();
	return 0;
}