// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"

const int buckets = 4;

struct Histogram {
	vector<int> blue;
	vector<int> green;
	vector<int> red;
};
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: create_histogram
// Purpose: 
// Inputs:
// - window_name
// - img
// Output: 
// Pre: 
// Post: 
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void create_histogram(const string wnd_name, cv::Mat src_img, cv::Mat target_img) {
	int dims[] = { buckets, buckets, buckets };
	cv::Mat histogram(3, dims, CV_32S, cv::Scalar::all(0));
	int bucket_size = 256 / buckets;

	for (int r = 0; r < src_img.rows; ++r) {
		for (int c = 0; c < src_img.cols; ++c) {
			cv::Vec3b& pixel = src_img.at<cv::Vec3b>(r, c); // Reference to BGR vector
			int blue = pixel[0], green = pixel[1], red = pixel[2];

			pixel[0] = 255 - pixel[0]; // Blue channel
			pixel[1] = 255 - pixel[1]; // Green channel
			pixel[2] = 255 - pixel[2]; // Red channel
		}
	}
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: run()
// Purpose: primary execution loop - opens two files, creates two windows of a pre and post analysis of the colour histogram
// Inputs: n/a
// Output: integer 0 or -1, success or failure, respectfully
// Pre: n/a
// Post: two windows one before and one after showing replacement of pixels.
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int run() {
	string foreground_file = "./foreground.jpg";
	string background_file = "./background.jpg";
	string wnd_foreground = "CSS587 - Assignment 2";
	string wnd_background = "CSS587 - Assignment 2 - Histogram Processed";
	cv::Mat foreground_img, background_img;

	try {
		foreground_img = cv::imread(foreground_file);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << foreground_file << "\". Reason: " << e.msg << endl;
		return -1;
	}
	try {
		background_file = cv::imread(background_file);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << background_file << "\". Reason: " << e.msg << endl;
		return -1;
	}
	cv::namedWindow(wnd_main, cv::WINDOW_AUTOSIZE);
	create_histogram(wnd_main, foreground_img, background_file);
	cv::imshow(wnd_foreground, foreground_img);
	cv::imshow(wnd_background, background_file);

	// check for and handle the ESCape and Space
	if (cv::waitKey(0) == 27 || cv::waitKey(0) == 32)
		cv::destroyAllWindows();
	return 0;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: main
// Purpose: 
// Inputs:
// - window_name
// - img
// Output: 
// Pre: 
// Post: 
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int main() {
	return run();
}
