//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// Git: https://github.com/caolandix/CSS587_Assignments, Branch: CSS587_ASSIGNMENT1
// Assignment 1
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "CSS587_Assignments.h"
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: flip_image()
// Purpose: Processes the image by flipping it
// Inputs:
//	- img: the original image before flipping
//  - img_flip: a reference tot he img_flip matrix
// Outputs: img_flip is passed by reference so the "output" is there and returned to the () caller.
// Pre Conditions: img is read in accurately
// Post Conditions: img is flipped horizontally then vertically
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void image_flip(const cv::Mat img, cv::Mat &img_flip) {
	cv::flip(img, img_flip, 0);
	cv::flip(img_flip, img_flip, 1);
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: grayscale_image()
// Purpose: Changes the colour to grayscale
// Inputs:
// - img: The image to cnvt to grayscale
// - img_grayscale: the grayscaled image matrix
// Outputs: grayscale is passed by reference so the "output" is there and returned to the () caller.
// Pre Conditions: img is flipped horizontally then vertically
// Post Conditions: img_grayscale contains the grayscaled img
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void image_grayscale(const cv::Mat img, cv::Mat &img_grayscale) {
	cv::cvtColor(img, img_grayscale, cv::COLOR_BGR2GRAY);
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: image_blur()
// Purpose: Smoothes out bad elements
// Inputs: 
// - img: source img
// - img_blur: reference to the image to blur
// - sigma1: a minimum sigma to affect the blur
// - sigma2: a maximum sigma to affect the blur
// Outputs: img_blur is passed by reference so the "output" is there and returned to the () caller.
// Pre Conditions: img is pre-flipped and grayscaled
// Post Conditions: img is Gaussian blurred
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void image_blur(const cv::Mat img, cv::Mat &img_blur, const float sigma1, const float sigma2) {
	cv::GaussianBlur(img, img_blur, cv::Size(0, 0), sigma1, sigma2);
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: image_edgedetection()
// Purpose: Performs an edge detection
// Inputs: 
// - img: blurred and grayscaled image to convert
// - img_edges: reference to the matrix of edges found in the image_blur function
// - low_thresh: the low end of the hysteresis procedure
// - high_thresh: the high end of the hysteresis procedure
// Outputs: img_edges is passed by reference so the "output" is there and returned to the () caller.
// Pre Conditions: 
// Post Conditions: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void image_edgedetection(const cv::Mat img, cv::Mat &img_edges, const int low_thresh, const int high_thresh) {
	cv::Canny(img, img_edges, low_thresh, high_thresh);
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: on_trackbar_blur()
// Purpose: Callback function that allows the user to blur the grayscale image
// Inputs: (Standard parameter set for Trackbar callback as defined by OpenCV)
// - sigma_slider: Current position of the slider
// - img: a void pointer to the cv::Mat image 
// Pre Conditions: image is gray-scaled, and possibly blurred already depending on if the slider was already adjusted
// Post Conditions: image is blurred, or not. Depends on whether the trackbar was changed
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void on_trackbar_blur(int sigma_slider, void* img) {
	cv::Mat grayed_img = *(cv::Mat *)img, blurred, img_edges;
	image_blur(grayed_img, blurred, sigma_slider, sigma_slider);
	image_edgedetection(blurred, img_edges, THRESHOLD_MIN, THRESHOLD_MAX);
	cv::imshow("CSS587 - Assignment 1 - Processed", img_edges);
	
	// Write out the edge detected image
	try {
		cv::imwrite("./output.jpg", img_edges);
	}
	catch (const cv::Exception& e) {
		cerr << "Error writing file \"./output.jpg\". Reason: " << e.msg << endl;
	}
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: process_contours()
// Purpose: Create a contour map and display it. Also 
// Parameters:
// - wnd: The output window to show it
// - img: the image to create contours by
// Pre Conditions: Already done the edge map in another window
// Post Conditions: Open another window with a contour map
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void process_contours(const string wnd, cv::Mat grayed_img) {
	cv::Mat blurred, img_edges, img_contoured;
	image_blur(grayed_img, blurred, 2.0, 2.0);
	image_edgedetection(blurred, img_edges, THRESHOLD_MIN, THRESHOLD_MAX);

	// Create vectors to store the contours and hierarchy
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;

	// I tried all three options for how the contours are represented and settled om CHAIN_APPROX_SIMPLE because they didn't make
	// a difference. Also tried multiple RETR_* and same, no real changes
	cv::findContours(img_edges, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	
	// Draw the contours on a blank canvas (or on the original image)
	cv::Mat drawing = cv::Mat::zeros(img_edges.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(196, 196, 0); // Cyan color
		
		// -1 as the third argument draws all contours; 'i' draws a specific index
		// NOTE: Changed third arg to -1 and it took much much longer to draw, but it still looked the same.
		cv::drawContours(drawing, contours, static_cast<int>(i), color, 1, cv::LINE_8, hierarchy, 0);
	}
	cv::imshow(wnd, drawing);

	// Write out the contoured image
	try {
		cv::imwrite("./contours.jpg", drawing);
	}
	catch (const cv::Exception& e) {
		cerr << "Error writing file \"./contours.jpg\". Reason: " << e.msg << endl;
	}
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: create_histogram_of_many_colors()
// Purpose: Create a histogram of the RGB planes 
// Parameters:
// - wnd: The output window to show it
// - img: the image to create the RGB planes by
// Pre Conditions: unaltered original image
// Post Conditions: Open another window with a n RGB histogram
// NOTES: This was not my code. I wanted to see a histogram and so I lifted it from the internet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void create_histogram_of_many_colors(const string wnd, const cv::Mat img) {
	vector<cv::Mat> bgr_planes;
	cv::split(img, bgr_planes);

	int hist_size = 256;         // Number of bins (0-255)
	float range[] = { 0, 256 }; // Upper boundary is exclusive (0 to 255)
	const float* hist_range[] = { range };
	bool uniform = true;
	bool accumulate = false;
	// Output matrices for the histograms
	cv::Mat b_hist, g_hist, r_hist;

	// Calculate the histograms for each channel
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size, hist_range, uniform, accumulate);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size, hist_range, uniform, accumulate);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size, hist_range, uniform, accumulate);

	// Create an canvas image to draw the histograms
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / hist_size);
	cv::Mat hist_img(hist_h, hist_w, CV_8UC3, cv::Scalar(20, 20, 20)); // Dark background

	// Normalize the results to fit the canvas height
	cv::normalize(b_hist, b_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(g_hist, g_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(r_hist, r_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());
	
	// Draw the histograms onto the canvas
	for (int i = 1; i < hist_size; i++) {
		// Draw Blue channel histogram
		cv::line(hist_img,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			cv::Scalar(255, 0, 0), 2, cv::LINE_AA);

		// Draw Green channel histogram
		cv::line(hist_img,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

		// Draw Red channel histogram
		cv::line(hist_img,
			cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
	}
	cv::imshow(wnd, hist_img);
	
	// Write out the contoured image
	try {
		cv::imwrite("./hist_colours.jpg", hist_img);
	}
	catch (const cv::Exception& e) {
		cerr << "Error writing file \"./hist_colours.jpg\". Reason: " << e.msg << endl;
	}
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: run()
// Purpose: flip the image both vertically and horizontally (180 degree rotation), convert it to greyscale, blur it, and 
//	detect edges. 
// Parameters:
//  - filename: a string containing the path of the image file to load.
// Pre Conditions: There's an image file to read
// Post Conditions: Application closes without crashing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int run(const string filename) {
	string wnd_primary = "CSS587 - Assignment 1 - Processed";
	string wnd_2ndaryimage_mods = "Image Contours";
	string wnd_histogram_colors = "Colour Histogram";
	string target_filename = "./output.jpg";
	cv::Mat img, img_flip, img_grayscale, img_blur, img_edges;
	int low_thresh = THRESHOLD_MIN, high_thresh = THRESHOLD_MAX, curr_thresh_pos = low_thresh;

	// handle reading in the original image
	try {
		img = cv::imread(filename, -1);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << endl;
		return -1;
	}
	if (img.empty()) {
		cout << "OpenCV::imread(): Image is empty." << endl;
		return -1;
	}

	// Creating the main windows for the various outputs
	cv::namedWindow(wnd_primary, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(wnd_2ndaryimage_mods, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(wnd_histogram_colors, cv::WINDOW_AUTOSIZE);

	// Flip the image twice
	image_flip(img, img_flip);

	// Convert to Grayscale
	image_grayscale(img_flip, img_grayscale);

	// Do Edge Detection
	image_edgedetection(img_grayscale, img_edges, THRESHOLD_MIN, THRESHOLD_MAX);

	// Gaussian blur the image and create the trackbar which in turn creates the edgelines and displays them. 
	int sigma1 = SIGMA_MIN;
	cv::createTrackbar("Blur Sigma", wnd_primary, &sigma1, SIGMA_MAX, on_trackbar_blur, &img_grayscale);
	cv::setTrackbarMin("Blur Sigma", wnd_primary, SIGMA_MIN);

	// Adding contour points and displaying them.
	process_contours(wnd_2ndaryimage_mods, img_grayscale);

	// Create a display a histogram of colors for the image
	create_histogram_of_many_colors(wnd_histogram_colors, img);

	// Exit on Escape, space, or window close
	if (cv::waitKey(0) == 27 || cv::waitKey(0) == 33) {
		cv::destroyAllWindows();
		return 0;
	}
	return 0;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Purpose: entry point of execution
// Inputs: standard command-line arg handing. Expecting a single item as the filename
// Outputs: -1 on failure, 0 on success
// Precondition: n/a
// postcondition: does not crash on exit.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char **argv) {
	if (argc == 2)
		return run(argv[1]);
	else
		cout << "Command Line args: <filename-to-process>" << endl;
	return -1;
}
