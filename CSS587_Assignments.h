////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSS587_Assignments.h : preprocessor includes, function prototypes, preprocessor constants
// Author: Caolan O'Domhnaill
// Git: https://github.com/caolandix/CSS587_Assignments, Branch: CSS587_ASSIGNMENT1
// Assignment 1
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef CSS587_ASSIGNMENTS_H
#define CSS587_ASSIGNMENTS_H

#include <iostream>
#include <string>
#include <vector>

// OpenCV Headers
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Namespaces
// NOTE: Not using cv because I want to explicitly call out those members and methods of the OpenCV library
using namespace std;
//
// Preprocessor Constants
//
// Blur constants
#define SIGMA_MIN 2
#define SIGMA_MAX 10
// Edge detection constants
#define THRESHOLD_MIN 20
#define THRESHOLD_MAX 60


// Function prototypes. Modern C++ doesn't need this but I like to maintain it
int run(const string);
void image_flip(const cv::Mat, cv::Mat&);
void image_grayscale(const cv::Mat, cv::Mat&);
void image_blur(const cv::Mat, cv::Mat&,const float, const float);
void image_edgedetection(const cv::Mat, cv::Mat&, const int, const int);
void on_trackbar_blur(int, void*);
void process_contours(const string, const cv::Mat);
void create_histogram_of_many_colors(const string, const cv::Mat);

#endif