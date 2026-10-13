// CSS587_Assignments.cpp : Defines the entry point for the application.
// Author: Caolan O'Domhnaill
// 
#include "CSS587_Assignments.h"

class AppMgr {
public:
	AppMgr() {};
	~AppMgr() {};

	// Member methods
	void Ex2_4();

	void setCap(const enum cv::VideoCaptureProperties prop, const int pos) { m_cap.set(prop, pos); };
	bool getDontSet() const { return m_dontset; };
	void setDontSet(const bool val) { m_dontset = val; };
	void setRun(const int val) { m_run = val; };


	// make the callback a member of the class by makign it static and allowing the class instance to be passed as userdata
	static void onTrackbarSlide(int pos, void *userdata) {
		AppMgr *pinstance = static_cast<AppMgr *>(userdata);
		if (pinstance) {
			pinstance->setCap(cv::CAP_PROP_POS_FRAMES, pos);
			if (!pinstance->getDontSet())
				pinstance->setRun(1);
			pinstance->setDontSet(false);
		}
	}

private:

	// member variables for handling the slider
	int m_slider_position = 0;
	int m_run = 1;
	bool m_dontset = false;
	string m_videoFilePath = "C:/Users/Caolan/source/repos/CSS587_Assignments/images/TokyoDrift.mp4";
	cv::VideoCapture m_cap;
};

void AppMgr::Ex2_4() {
	string windowName = "OpenCV Book - Example 2-4";
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
	m_cap.open(m_videoFilePath);
	int frames = (int)m_cap.get(cv::CAP_PROP_FRAME_COUNT);
	int tmp_w = (int)m_cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int tmp_h = (int)m_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	cout << "Video has " << frames << " frames of dimensions(" << tmp_w << ", " << tmp_h << ")." << endl;
	cv::createTrackbar("Position", windowName, &m_slider_position, frames, &onTrackbarSlide);
	cv::Mat frame;
	for (;;) {
		if (m_run != 0) {
			m_cap >> frame;
			if (frame.empty())
				break;
			int curr_pos = (int)m_cap.get(cv::CAP_PROP_POS_FRAMES);
			m_dontset = true;
			cv::setTrackbarPos("Position", windowName, curr_pos);
			cv::imshow(windowName, frame);
			m_run -= 1;
		}
		// Handle the X closure of the window so it doesn't crash
		if (cv::getWindowProperty(windowName, cv::WND_PROP_VISIBLE) < 1.0)
			break;

		char c = (char)cv::waitKey(10);
		switch (tolower(c)) {
		case 's':
		{
			m_run = 1;
			cout << "Single step mode, run = " << m_run << endl;
		}
		break;
		case 'r':
		{
			m_run = -1;
			cout << "run mode, run = " << m_run << endl;
		}
		break;
		case 27:
			break;
		}
	}
}

void OpenCVBookExamples() {
	AppMgr appMgr;
	appMgr.Ex2_4();
}

void assignment_1() {

}

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
	string filename = "";// args[1];
	string assignment = ""; //args[2];
	int assignment_num;
	int ret_val = 0;

	try {
		 //assignment_num = stoi(assignment);
		assignment_num = -1;
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
		case 1:
			assignment_1();
			break;
		case -1:
			OpenCVBookExamples();
			break;
		default:
			cerr << "Not a valid assignment number" << endl;
			ret_val = -1;
	}
	cv::destroyAllWindows();
	return ret_val;
}

int main(int argc, char **argv) {
	vector<string> args(argv, argv + argc);

	//if (argc > 2)
	if (true)
		return run(args);
	cout << "Command Line args: <filename-to-process> <assignment number>" << endl;
	return -1;
}
