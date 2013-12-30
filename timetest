#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
using namespace cv;

// cpp
time_t t_start;
time_t t_end;

Mat colorSegRGB(const Mat& src){
	assert(src.type() == CV_8UC3);
	Mat filtered_Image;
	inRange(src, Scalar(0, 0, 0), 
		     Scalar(200, 200, 200), filtered_Image);
	return filtered_Image;
}

Mat colorSegHSV(const Mat& src){
	assert(src.type() == CV_8UC3);
	Mat filtered_Image, HSV_Image;
	cvtColor(src, filtered_Image, CV_BGR2HSV);
	inRange(src, Scalar(0, 0, 0), 
		     Scalar(200, 200, 200), filtered_Image);
	return filtered_Image;
}

int main(int argc, char** argv){
		
	const char* filename = argc >= 2? argv[1] : "p1.jpg";
	
	auto start = std::chrono::high_resolution_clock::now();
	Mat input_Image = imread(filename, 1);
	auto finish = std::chrono::high_resolution_clock::now();
	cout << "Load a Image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";


	start = std::chrono::high_resolution_clock::now();
	Mat filtered_Image_RGB = colorSegRGB(input_Image);
	finish = std::chrono::high_resolution_clock::now();
	cout << "RGB colorSeg: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";

	start = std::chrono::high_resolution_clock::now();
	Mat filtered_Image_HSV = colorSegHSV(input_Image);
	finish = std::chrono::high_resolution_clock::now();
	cout << "HSV colorSeg: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";

	//imshow("opencvtest",input_Image);
    	waitKey(0);

   	return 0;
}
