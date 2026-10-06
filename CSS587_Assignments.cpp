// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"

int run() {
	cv::Mat frame;
	string filename = "C:/Users/Caolan/source/repos/CSS587_Template/images/warriors.jpg";
	string windowName = "Basic CSS587 Template Sample";

	try {
		frame = cv::imread(filename, -1);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	if (frame.empty()) {
		cout << "OpenCV::imread(): Image is empty." << endl;
		return -1;
	}

	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
	while (1) {

		// display the frame
		cv::imshow(windowName, frame);

		// check for and handle the ESCape key
		if (cv::waitKey(0) == 27)
			break;

		// Handle the X closure of the window so it doesn't crash
		if (cv::getWindowProperty(windowName, cv::WND_PROP_VISIBLE) < 1.0)
			break;
	}
	cv::destroyAllWindows();
	return 0;
}

int main() {
	return run();
}
