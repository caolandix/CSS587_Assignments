// CSS587_Template.h : Include file for standard system include files,
// or project specific include files.
#ifndef CSS587_ASSIGNMENTS_H
#define CSS587_ASSIGNMENTS_H

#include <iostream>
#include <string>

// OpenCV related
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

const int buckets = 4;

// Function prototypes
int run(const vector<string>);
void create_histogram(const string, cv::Mat, cv::Mat&);

#endif