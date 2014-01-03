#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <cmath>

#define MAX_FRAME_NUMBER 200

#define ms 1000000
#define us 1000
#define ns 1

#define ms_str "ms"
#define us_str "us"
#define ns_str "ns"


using namespace std;
using namespace cv;

time_t t_start;
time_t t_end;

float timeMatrix [MAX_FRAME_NUMBER][8];
int Frame_ctr = 0;
int para_ctr = 0;

int time_unit = us; // Modify unit of time here
string time_unit_string = us_str; // Also the string

int minRGB[3] = {122,10,10};
int maxRGB[3] = {240,50,50};
int minHSV[3] = {0,175,159};
int maxHSV[3] = {180,255,250};


void calculate_MeanStd(){
	
	float average[8],std[8];
	cout << "------------------- here? After Define param array-------------------" << endl;
	for(int ctr=0; ctr<8; ctr++){

		// Calculate Average
		for(int Fctr=0; Fctr<MAX_FRAME_NUMBER; Fctr++){
			average[ctr] += timeMatrix[Fctr][ctr];
		}
		cout << "------- here? After first average--------" << endl;
		average[ctr] = average[ctr]/MAX_FRAME_NUMBER;
		cout << "Average " << ctr << " : " << average[ctr] << time_unit_string << endl;

		// Calculate Stadard Deviation
		for(int Fctr=0; Fctr<MAX_FRAME_NUMBER; Fctr++){
			//std[ctr] += pow(timeMatrix[Fctr][ctr] - average[ctr],2);
			std[ctr] += (timeMatrix[Fctr][ctr]-average[ctr])*(timeMatrix[Fctr][ctr]-average[ctr]);
			//std[ctr] += timeMatrix[Fctr][ctr] - average[ctr];
		}
		cout << "------- here? After first std--------" << endl;
		std[ctr] = sqrt(std[ctr]/MAX_FRAME_NUMBER);
		cout << "stdDev " << ctr << " : " << std[ctr] << time_unit_string << endl;
	
	}
	int Pctr = 0;
	cout << "------------------- Result -------------------" << endl; 
	cout << "Time Consumption : " << " Average " << time_unit_string <<" |  Dev " << time_unit_string << " | " << " Per.   | " << endl;
	cout << "Capture an image : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "Show color image : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "Save color image : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "Define an image* : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "RGB Segmentation : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "HSV Segmentation : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "Show binary image: " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
	cout << "Overall Duration : " << average[Pctr] <<" | "<< std[Pctr] << " | "<< average[Pctr]*100/average[7] << "% | "<< endl; Pctr++;
}


Mat colorSegRGB(const Mat& src){
	assert(src.type() == CV_8UC3);
	Mat filtered_Image;
	inRange(src, Scalar(minRGB[2], minRGB[1], minRGB[0]), 
		     Scalar(maxRGB[2], maxRGB[1], maxRGB[0]), filtered_Image);
	return filtered_Image;
}

Mat colorSegHSV(const Mat& src){
	assert(src.type() == CV_8UC3);
	Mat filtered_Image, HSV_Image;
	cvtColor(src, filtered_Image, CV_BGR2HSV);
	inRange(src, Scalar(minHSV[0], minHSV[1], minHSV[2]), 
		     Scalar(maxHSV[0], maxHSV[1], maxHSV[2]), filtered_Image);
	return filtered_Image;
}

void eva_loop(VideoCapture camera){

	auto start = std::chrono::high_resolution_clock::now();
        Mat cameraFrame;
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	auto finish = std::chrono::high_resolution_clock::now();
	cout << "Capture an image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
	imshow("Camera",cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "Show a color image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
	imwrite( "./cameraImg.jpg", cameraFrame );
	finish = std::chrono::high_resolution_clock::now();
	cout << "Save a color image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
	Mat SegedRGBImg, SegedHSVImg;
	finish = std::chrono::high_resolution_clock::now();
	cout << "Define an image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
	SegedRGBImg = colorSegRGB(cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "RGB Segmentation: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
	SegedHSVImg = colorSegHSV(cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "HSV Segmentation: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
        imshow("Show a binary image",SegedRGBImg);
	finish = std::chrono::high_resolution_clock::now();
	cout << "Show a binary image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	

}

Mat redFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);

    Mat binaryImg;
    inRange(src, Scalar(50, 59, 23), Scalar(180, 150, 150), binaryImg);
	//cvInRangeS(imgHSV, cvScalar(170,160,60), cvScalar(180,256,256), imgThresh); 
    return binaryImg;
}

int main(int argc, char** argv) {
    int cameraNumber = 0;
    float time_open_1st2nd3rdframe[4];

    auto start = std::chrono::high_resolution_clock::now();
    if (argc>1)
        cameraNumber = atoi(argv[1]);
    VideoCapture camera;
    camera.open(cameraNumber);
    if(!camera.isOpened()) {
        cerr<<"Error opening camera"<<std::endl;
        exit(1);
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH,640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT,480);

	auto finish = std::chrono::high_resolution_clock::now();
	cout << "Open Camera: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	time_open_1st2nd3rdframe[0] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	Mat cameraFrame;
	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture First Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	time_open_1st2nd3rdframe[1] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture Second Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	time_open_1st2nd3rdframe[2] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;

	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture third Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;
	time_open_1st2nd3rdframe[3] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit;


    while(Frame_ctr < MAX_FRAME_NUMBER) {
	cout << "------------- Next Frame: " << Frame_ctr << "--------------"<< endl;
	auto start1 = std::chrono::high_resolution_clock::now();
        eva_loop(camera);
	auto finish1 = std::chrono::high_resolution_clock::now();
	cout << "Overall: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish1-start1).count())/time_unit << " "<< time_unit_string << endl;
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish1-start1).count())/time_unit;
        
	char keypress = cv::waitKey(20);
        if (keypress==27) {
            break;
        }
	Frame_ctr++;
	para_ctr = 0;
    }
	cout << "------------------- here? Before Entering cal_Meanstd() -------------------" << endl;

	cout << endl;
	calculate_MeanStd();

	cout << endl;
	cout << "---------- Open camera & 1st 2nd 3rd Frame -------------------" << endl; 
	cout << "Time Consumption : " << " Average " << time_unit_string <<" | "<< endl;
	cout << "Open Camera      : " << time_open_1st2nd3rdframe[0] << endl;
	cout << "1st Frame        : " << time_open_1st2nd3rdframe[1] << endl;
	cout << "2nd Frame        : " << time_open_1st2nd3rdframe[2] << endl;
	cout << "3rd Frame        : " << time_open_1st2nd3rdframe[3] << endl;

    return 0;
}
