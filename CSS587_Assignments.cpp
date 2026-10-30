// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: doSIFTDetector()
// Inputs:
// - img_src1: 1st pic to compare vs 2nd pic
// - img_src2: 2nd pic to compare vs 1st pic
// - img_output: Reference to output Mat
// Output:
// - img_output: Updated output image with matches
// Preconditions: images 1st and 2nd are loaded
// PostConditions: output with matches are put together.
// Notes:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void doSIFTDetector(const cv::Mat img_src1, const cv::Mat img_src2, cv::Mat& img_output) {
	vector<cv::KeyPoint> keypoints1, keypoints2;
	vector<vector<cv::DMatch>> knn_matches;
	cv::Mat descriptors1, descriptors2;

	// Matchmaker matchmaker make me a, image.
	cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
	cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);

	detector -> detectAndCompute(img_src1, cv::noArray(), keypoints1, descriptors1);
	detector -> detectAndCompute(img_src2, cv::noArray(), keypoints2, descriptors2);

	// Brute force matcher
	matcher -> knnMatch(descriptors1, descriptors2, knn_matches, 2);

	// Adjusted the thresh ratio from 0.01 to 0.999999
	// It didn't start creating lines until between 0.10 and 0.15
	// 0.999999 had so many lines you couldn't see the images. All numbers in between, in chunks of 0.05, added more or less lines depending how large the number was. 
	const float ratio_thresh = 0.70f;
	vector<cv::DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++) {
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
			good_matches.push_back(knn_matches[i][0]);
	}

	// Draw the matches.
	cv::drawMatches(
		img_src1, keypoints1,
		img_src2, keypoints2,
		good_matches,
		img_output,
		cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: doSURFDetector()
// Inputs:
// - img_src1: 1st pic to compare vs 2nd pic
// - img_src2: 2nd pic to compare vs 1st pic
// - img_output: Reference to output Mat
// Output:
// - img_output: Updated output image with matches
// Preconditions: images 1st and 2nd are loaded
// PostConditions: output with matches are put together.
// Notes:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void doSURFDetector(const cv::Mat img_src1, const cv::Mat img_src2, cv::Mat& img_output) {

	// Detect the keypoints using SURF Detector, compute the descriptors
	int minHessian = 400;
	cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(minHessian);
	vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	detector -> detectAndCompute(img_src1, cv::noArray(), keypoints1, descriptors1);
	detector -> detectAndCompute(img_src2, cv::noArray(), keypoints2, descriptors2);

	// Step 2: Matching descriptor vectors with a FLANN based matcher
	// Since SURF is a floating-point descriptor NORM_L2 is used
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	vector<vector<cv::DMatch>> knn_matches;
	matcher -> knnMatch(descriptors1, descriptors2, knn_matches, 2);

	// Filter matches using the Lowe's ratio test
	const float ratio_thresh = 0.7f;
	std::vector<cv::DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++) {
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
			good_matches.push_back(knn_matches[i][0]);
	}

	// Draw matches
	cv::drawMatches(
		img_src1, keypoints1,
		img_src2, keypoints2,
		good_matches,
		img_output,
		cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
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
	string image_file1 = "C:/Users/Caolan/source/repos/CSS587_Assignments/kittens1.jpg";
	string image_file2 = "C:/Users/Caolan/source/repos/CSS587_Assignments/kittens2.jpg";
	string wnd_sift = "CSS587 - Assignment 3 - SIFT";
	string wnd_surf = "CSS587 - Assignment 3 - SURF";
	cv::Mat img_kittens1, img_kittens2, img_output1, img_output2;

	try {
		img_kittens1 = cv::imread(image_file1);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << image_file1 << "\". Reason: " << e.msg << endl;
		return -1;
	}
	try {
		img_kittens2 = cv::imread(image_file2);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << image_file2 << "\". Reason: " << e.msg << endl;
		return -1;
	}
	cv::namedWindow(wnd_sift, cv::WINDOW_NORMAL);
	cv::namedWindow(wnd_surf, cv::WINDOW_NORMAL);

	doSIFTDetector(img_kittens1, img_kittens2, img_output1);
	cv::imshow(wnd_sift, img_kittens1);
	cv::resizeWindow(wnd_sift, img_output1.cols / scale, img_output1.rows / scale);
	cv::imshow(wnd_sift, img_output1);

	doSURFDetector(img_kittens1, img_kittens2, img_output2);
	cv::imshow(wnd_surf, img_kittens1);
	cv::resizeWindow(wnd_surf, img_output2.cols / scale, img_output2.rows / scale);
	cv::imshow(wnd_surf, img_output2);

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
