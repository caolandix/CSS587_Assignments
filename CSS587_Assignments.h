// CSS587_Template.h : Include file for standard system include files,
// or project specific include files.
#ifndef CSS587_ASSIGNMENTS_H
#define CSS587_ASSIGNMENTS_H

#include <iostream>
#include <string>
#include <vector>

// OpenCV related
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

const int scale = 8;

// Function prototypes
int run(const vector<string>);
void doSURFDetector(const cv::Mat, const cv::Mat, cv::Mat&);
void doSIFTDetector(const cv::Mat, const cv::Mat, cv::Mat&);

#endif