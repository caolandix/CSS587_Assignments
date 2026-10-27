// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: create_histogram
// Purpose: Creates the histogram table
// Inputs:
// - wnd_name: the name of the window we're outputting to
// - src_img: The source image we're building the histogram from
// - tgt_img: the target image we're writing the image to after it has been modded by the "greenscreen" colour
// Output: tgt_img
// Pre: 
// Post: 
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void create_histogram(const string wnd_name, cv::Mat src_img, cv::Mat &tgt_img) {
	int dims[] = { buckets, buckets, buckets };
	cv::Mat histogram(3, dims, CV_32S, cv::Scalar::all(0));
	int bucket_size = 256 / buckets;
	vector<int> bucket_cnts = { 0, 0, 0, 0 };

	for (int row = 0; row < src_img.rows; ++row) {
		for (int col = 0; col < src_img.cols; ++col) {
			cv::Vec3b& pixel = src_img.at<cv::Vec3b>(row, col); // Reference to BGR vector

			// Figure out the bucket to increment
			int blue_bucket = pixel[0] / bucket_size, green_bucket = pixel[1] / bucket_size, red_bucket = pixel[2] / bucket_size;
			bucket_cnts.at(blue_bucket)++;
			bucket_cnts.at(green_bucket)++;
			bucket_cnts.at(red_bucket)++;
			histogram.at<cv::Vec3b>(col, row)[0] = blue_bucket;		// Blue channel
			histogram.at<cv::Vec3b>(col, row)[1] = green_bucket;	// Green channel
			histogram.at<cv::Vec3b>(col, row)[2] = red_bucket;		// Red Channel
		}
	}
	// Dereference the iterator to get the value
	auto max_it = max_element(bucket_cnts.begin(), bucket_cnts.end());
	int max_val = *max_it;


	// Loop through the 
	for (int row = 0; row < src_img.rows; ++row) {
		for (int col = 0; col < src_img.cols; ++col) {
			cv::Vec3b& pixel = src_img.at<cv::Vec3b>(row, col); // Reference to BGR vector

			// Figure out the bucket to increment
			int blue_bucket = pixel[0] / bucket_size, green_bucket = pixel[1] / bucket_size, red_bucket = pixel[2] / bucket_size;
			
			// Find out what the pixel is close to and assign it a new value which then gets put into the background image
			int cBlue = pixel[0] * bucket_size + bucket_size / 2;
			int cGreen = pixel[1] * bucket_size + bucket_size / 2;
			int cRed = pixel[2] * bucket_size + bucket_size / 2;

			blue_bucket = histogram.at<cv::Vec3b>(col, row)[0];		// Blue channel
			green_bucket = histogram.at<cv::Vec3b>(col, row)[1];// Green channel
			red_bucket = histogram.at<cv::Vec3b>(col, row)[2];		// Red Channel

			tgt_img.at<cv::Vec3b>(col, row)[0] = blue_bucket;		// Blue channel
			tgt_img.at<cv::Vec3b>(col, row)[1] = green_bucket;	// Green channel
			tgt_img.at<cv::Vec3b>(col, row)[2] = red_bucket;		// Red Channel
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
// Post: two windows one before and one after showing replacement of pixels. Output to overlay.jpg
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int run() {
	string foreground_file = "./foreground.jpg";
	string overlay_file = "./overlay.jpg";
	string wnd_fg = "CSS587 - Assignment 2 - Foreground";
	string wnd_bg = "CSS587 - Assignment 2 - Background";
	string wnd_overlay = "CSS587 - Assignment 2 - Histogram Processed";
	cv::Mat fg_img, bg_img;

	try {
		fg_img = cv::imread(foreground_file);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << foreground_file << "\". Reason: " << e.msg << endl;
		return -1;
	}
	cv::namedWindow(wnd_fg, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(wnd_bg, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(wnd_overlay, cv::WINDOW_AUTOSIZE);
	create_histogram(wnd_fg, fg_img, bg_img);
	cv::imshow(wnd_fg, fg_img);
	cv::imshow(wnd_bg, bg_img);
	cv::imshow(wnd_overlay, bg_img);
	try {
		cv::imwrite(overlay_file, bg_img);
	}
	catch (const cv::Exception& e) {
		cerr << "Error writing file \"" << overlay_file << "\". Reason: " << e.msg << endl;
		return -1;
	}
	// check for and handle the ESCape and Space
	if (cv::waitKey(0) == 27 || cv::waitKey(0) == 32)
		cv::destroyAllWindows();
	return 0;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: main
// Purpose: Start of execution
// Inputs: n/a
// Output: 0 or -1 for success or error
// Pre: n/a
// Post: n/a
// Notes:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int main() {
	return run();
}
