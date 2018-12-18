#include <memory.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define USHORT_MAX				(0xFFFF)
#define UCHAR_MAX				(0xFF)
#define MIN_DEPTH				(400)			// minimun reliable depth value of Kinect
#define MAX_DEPTH				(16383)			// maximun reliable depth value of Kinect
#define UNKNOWN_DEPTH			(0)
#define UNKNOWN_DEPTH_COLOR		(0x00000000)
#define NEAREST_COLOR			(0x00000000)
#define TOO_FAR_COLOR			(0x000000FF)
#define MINIMUN(a,b)			((a) < (b) ? (a) : (b))

using namespace std;
using namespace cv;


// look-up table for generating log-scaled depth map for visualization
unsigned char depth_color_table[USHRT_MAX + 1];
unsigned char GetIntensity(int depth);
int InitializeDepthColorTable(void);
int FillDepthHoles(const cv::Mat& src_depth, cv::Mat& filled_depth);
int AlignDepthWithColor(const cv::Mat& src_depth, const cv::Mat& src_color, cv::Mat& aligned_depth);


int main(int argc, char** argv)
{
	string im_file_color = "C:\\Users\\63229\\Desktop\\samples\\left_247.jpg";
	string im_file_depth = "C:\\Users\\63229\\Desktop\\samples\\raw_depth_247.xml";
	
	cv::Mat im_color, im_color_resized, im_raw_depth, im_aligned_depth;
	FileStorage fs(im_file_depth, FileStorage::READ);


	InitializeDepthColorTable();
	fs["Mat"] >> im_raw_depth;
	im_color = imread(im_file_color);
	resize(im_color, im_color_resized, im_raw_depth.size(), 0, 0, INTER_LINEAR);

	AlignDepthWithColor(im_raw_depth, im_color_resized, im_aligned_depth);

	imshow("color", im_color_resized);
	char keyval = waitKey(0);

	destroyAllWindows();
	return 0;
}


unsigned char GetIntensity(int depth)
{
	// Validate argument
	if (depth < MIN_DEPTH || depth > MAX_DEPTH) return UCHAR_MAX;

	// Use a logarithmic scale that shows more detail for nearer depths.
	// The constants in this formula were chosen such that values between
	// MIN_DEPTH and MAX_DEPTH will map to the full range of possbile byte values.
	const float depth_range_scale = 500.0f;
	const int intensity_range_scale = 74;
	return (unsigned char)(~(unsigned char) MINIMUN(UCHAR_MAX, 
		log((double)(depth-MIN_DEPTH) / depth_range_scale + 1) * intensity_range_scale));
}


int InitializeDepthColorTable(void)
{
	memset(depth_color_table, 0, USHORT_MAX + 1);
	// set color for unkown depth
	depth_color_table[UNKNOWN_DEPTH] = UNKNOWN_DEPTH_COLOR;

	unsigned short min_reliable_depth = MIN_DEPTH;
	unsigned short max_reliable_depth = MAX_DEPTH;

	for (int depth = UNKNOWN_DEPTH + 1; depth < min_reliable_depth; ++depth)
	{
		depth_color_table[depth] = NEAREST_COLOR;
	}

	for (unsigned short depth = min_reliable_depth; depth <= max_reliable_depth; ++depth)
	{
		unsigned char intensity = GetIntensity(depth);
		depth_color_table[depth] = 255 - intensity;
	}

	return 0;
}

int FillDepthHoles(const cv::Mat& src_depth, cv::Mat& filled_depth)
{
	src_depth.copyTo(filled_depth);
	double lambda = 0.25;
	double k = 25.0;

	int col_offset = 1;
	int row_offset = 3;

	for (int col = col_offset; col < src_depth.cols; ++col) 
	{
		for (int row = row_offset; row < src_depth.rows; ++row) 
		{
			// centered around the up up pixel
			ushort* ptr_filled_depth = filled_depth.ptr<ushort>(row);
			ushort* ptr_temp_up_three = filled_depth.ptr<ushort>(row - 3);
			ushort* ptr_temp_up_two = filled_depth.ptr<ushort>(row - 2);
			ushort* ptr_temp_up_one = filled_depth.ptr<ushort>(row -1);

			if (ptr_filled_depth[col] < 10) {
				double ni = static_cast<double>(ptr_temp_up_three[col] - ptr_temp_up_two[col]);
				double si = static_cast<double>(ptr_temp_up_one[col] - ptr_temp_up_two[col]);
				double wi = static_cast<double>(ptr_temp_up_two[col - 1] - ptr_temp_up_two[col]);
				double ei = static_cast<double>(ptr_temp_up_two[col + 1] - ptr_temp_up_two[col]);

				double cn = exp(-(ni * ni) / (k * k));
				double cs = exp(-(si * si) / (k * k));
				double cw = exp(-(wi * wi) / (k * k));
				double ce = exp(-(ei * ei) / (k * k));

				ptr_filled_depth[col] = static_cast<ushort>( ptr_temp_up_two[col] + lambda * (cn * ni + cs * si + cw * wi + ce * ei) + 0.5);
			}
		}
	}

	return 0;
}

int AlignDepthWithColor(const cv::Mat& src_depth, const cv::Mat& src_color, cv::Mat& aligned_depth)
{
	//// initialize the intrinsic and extrinsic parameters of the depth sensor of Kinect and color camera
	// Intrinsic
	static double color_fx = 810.1948; static double color_fy = 814.2451;
	static double color_u0 = 329.0910; static double color_v0 = 244.0002;

	static double depth_fx = 583.9147; static double depth_fy = 586.7294;
	static double depth_u0 = 328.9980; static double depth_v0 = 251.6837;
	
	// Extrinsic
	static cv::Mat rotation_matrix = (cv::Mat_<double>(3, 3) << 0.9997, 0.0200, 0.0158, -0.0200, 0.9998, 0.0035, -0.0157, -0.0038, 0.9999);
	static cv::Mat translation_matrix = (cv::Mat_<double>(3, 1) << 10, 53.8973, 40.4947);

	int depth_map_height = src_color.rows;
	int depth_map_width = src_color.cols;

	cv::Mat temp_depth_for_color = cv::Mat::zeros(depth_map_height, depth_map_width, CV_16UC1);

	for (int row_idx_depth = 0; row_idx_depth < depth_map_height; ++row_idx_depth)
	{
		const ushort* ptr_raw_depth_map = src_depth.ptr<ushort>(row_idx_depth);

		for (int col_idx_depth = 0; col_idx_depth < depth_map_width; ++col_idx_depth)
		{
			double raw_depth_val = static_cast<double>(ptr_raw_depth_map[col_idx_depth]);
			const double z_depth = raw_depth_val;
			const double x_depth = (col_idx_depth - depth_u0) * z_depth / depth_fx;
			const double y_depth = (row_idx_depth - depth_v0) * z_depth / depth_fy;

			cv::Mat xyz_depth = (cv::Mat_<double>(3, 1) << x_depth, y_depth, z_depth);
			cv::Mat xyz_color = rotation_matrix * xyz_depth + translation_matrix;

			const double z_color = xyz_color.at<double>(2, 0);
			const double y_color = xyz_color.at<double>(1, 0);
			const double x_color = xyz_color.at<double>(0, 0);

			double temp_col_idx_color = (x_color / z_color) * color_fx + color_u0;
			double temp_row_idx_color = (y_color / z_color) * color_fy + color_v0;

			int col_idx_color = static_cast<int>(temp_col_idx_color + 0.5);
			int row_idx_color = static_cast<int>(temp_row_idx_color + 0.5);
			if (col_idx_color < 0) col_idx_color = 0;
			if (col_idx_color >= depth_map_width) col_idx_color = (depth_map_width - 1);
			if (row_idx_color < 0) row_idx_color = 0;
			if (row_idx_color >= depth_map_height) row_idx_color = (depth_map_height - 1);

			// obtain depth value for color
			ushort* ptr_temp_depth_for_color = temp_depth_for_color.ptr<ushort>(row_idx_color);
			ptr_temp_depth_for_color[col_idx_color] = z_color;
		}
	}

	cv::Mat temp_dilated_depth;
	cv::Mat elemet = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::dilate(temp_depth_for_color, temp_dilated_depth, elemet);

	FillDepthHoles(temp_dilated_depth, aligned_depth);


	// Get the depth map to be shown
	cv::Mat depth_for_color_shown = cv::Mat::zeros(depth_map_height, depth_map_width, CV_8UC1);
	cv::Mat filled_depth_shown = cv::Mat::zeros(depth_map_height, depth_map_width, CV_8UC1);

	for (int row_idx = 0; row_idx < depth_map_height; ++row_idx)
	{
		uchar* ptr_depth_for_color_shown = depth_for_color_shown.ptr<uchar>(row_idx);
		ushort* ptr_depth_for_color = temp_depth_for_color.ptr<ushort>(row_idx);

		uchar* ptr_filled_depth_shown = filled_depth_shown.ptr<uchar>(row_idx);
		ushort* ptr_filled_depth = aligned_depth.ptr<ushort>(row_idx);

		for (int col_idx = 0; col_idx < depth_map_width; ++col_idx)
		{
			ushort depth_for_color_val = ptr_depth_for_color[col_idx];
			ptr_depth_for_color_shown[col_idx] = depth_color_table[depth_for_color_val] & 0x000000FF;

			ushort filled_depth_val = ptr_filled_depth[col_idx];
			ptr_filled_depth_shown[col_idx] = depth_color_table[filled_depth_val] & 0x000000FF;
		}
	}

	cv::imwrite("mapped.jpg", depth_for_color_shown);
	cv::imwrite("filled.jpg", filled_depth_shown);

	return 0;
}
