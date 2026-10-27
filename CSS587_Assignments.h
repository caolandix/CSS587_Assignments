// CSS587_Template.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// TODO: Reference additional headers your program requires here.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

const int buckets = 4;

// Function prototypes
int run(const vector<string>);
void create_histogram(const string, cv::Mat, cv::Mat&);