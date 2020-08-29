#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void detectSigns(Mat img, Mat original, String main_Screen, String sign_screen);
CascadeClassifier Detection;
char input_path[100];

int main(int argc, const char *argv[]) {
	// Settings
	int camera_index = 0;
	String window_name = "Road";
	String sign_screen = "Speed Limit";
	Rect myROI(640, 120, 639, 600);
	Mat test;
	imwrite("program_executed.JPG", test);
	if (!Detection.load("Speed_limit_classifier.xml")) {
		cout << "\n XML File not found";
		exit(0);
	}
	
	VideoCapture video(camera_index);
	video.set(CAP_PROP_FRAME_WIDTH, 1280.0);
	video.set(CAP_PROP_FRAME_HEIGHT, 720.0);

	if (video.isOpened() == false)
	{
		cout << "Error in camera" << endl;
		cin.get();
		return -1;
	}

	namedWindow(window_name, WINDOW_AUTOSIZE);

	while (true)
	{
		Mat frame;
		bool bSuccess = video.read(frame);
		
		if (bSuccess == false)
		{
			cout << "Camera disconnected" << endl;
			break;
		}
		
		Mat smaller_frame, gray_frame, lower_quality_gray_frame;
		// Crop frame
		smaller_frame = frame(myROI);
		// Gray scaled frame
		cv::cvtColor(smaller_frame, gray_frame, cv::COLOR_BGR2GRAY);
		// Lower quality
		resize(gray_frame, lower_quality_gray_frame, lower_quality_gray_frame.size(), 1,1 , INTER_LINEAR);
		//Run detect sign
		detectSigns(lower_quality_gray_frame, frame, window_name, sign_screen);

		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}
	return 0;
}

void detectSigns(Mat img, Mat original, String main_Screen, String sign_screen) {

	vector<Rect> faces;

	Detection.detectMultiScale(img, faces);

	for (int i = 0; i < faces.size(); i++) {
		Point pt1(faces[i].x + 640, faces[i].y + 120);
		Point pt2((faces[i].x + 640 + faces[i].height), (faces[i].y + faces[i].width + 120));

		rectangle(original, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);

		namedWindow(sign_screen, WINDOW_NORMAL);
		Rect myROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);

		Mat sign_frame = img(myROI);
		imshow(sign_screen, sign_frame);

	}
	Point pt1_scan(640, 120);
	Point pt2_scan(640+639,120+480);
	rectangle(original, pt1_scan, pt2_scan, Scalar(0, 255, 0) , 2, 8, 0);
	imshow(main_Screen, original);
}
