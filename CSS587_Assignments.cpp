// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"

void assignment_0(const string filename) {
	string windowName = "CSS587 - Assignment 0";
	cv::Mat frame;

	try {
		frame = cv::imread(filename, -1);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	if (frame.empty()) {
		cout << "OpenCV::imread(): Image is empty." << endl;
		return;
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

}

//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Central execution - Pattern used is factory - Very basic factory
// Notes: C++ Actually has the basics to support such a creation using <functional> and <unordered_map>. I may expand on this
// later usign sample code from the internet.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int run(const vector<string> args) {
	string filename = args[1];
	string assignment = args[2];
	int assignment_num;

	try {
		 assignment_num = stoi(assignment);
	}
	catch (const invalid_argument& e) {
		cerr << "Invalid input: No numeric conversion could be performed. (" << e.what() << ")\n";
	}
	catch (const out_of_range& e) {
		cerr << "Overflow error: The number is out of range for an int. (" << e.what() << ")\n";
	}
	catch (const exception& e) {
		cerr << "An unexpected standard exception occurred: " << e.what() << "\n";
	}

	switch (assignment_num) {
		case 0 :
			assignment_0(filename);
			break;
		default:
			cout << "That assignment number is unsupported at this time" << endl;
			return -1;
	}
	cv::destroyAllWindows();
	return 0;
}

int main(int argc, char **argv) {
	vector<string> args(argv, argv + argc);

	if (argc > 2)
		return run(args);
	cout << "Command Line args: <filename-to-process> <assignment number>" << endl;
	return -1;
}
