#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <cmath>

using namespace std;
using namespace cv;

time_t t_start;
time_t t_end;

float timeMatrix [200][8];
int Frame_ctr = 0;
int para_ctr = 0;

int minRGB[3] = {70,15,15};
int maxRGB[3] = {120,30,45};
int minHSV[3] = {0,0,40};
int maxHSV[3] = {26,40,255};


void calculate_MeanStd(){
	
	float average[8],std[8];
	cout << "------------------- here? After Define param array-------------------" << endl;
	for(int ctr=0; ctr<8; ctr++){

		// Calculate Average
		for(int Fctr=0; Fctr<200; Fctr++){
			average[ctr] += timeMatrix[Fctr][ctr];
		}
		cout << "------- here? After first average--------" << endl;
		average[ctr] = average[ctr]/200;

		// Calculate Stadard Deviation
		for(int Fctr=0; Fctr<200; Fctr++){
			//std[ctr] += pow(timeMatrix[Fctr][ctr] - average[ctr],2);
			//std[ctr] += (timeMatrix[Fctr][ctr]-average[ctr])*(timeMatrix[Fctr][ctr]-average[ctr]);
			std[ctr] += timeMatrix[Fctr][ctr] - average[ctr];
		}
		cout << "------- here? After first std--------" << endl;
		std[ctr] = sqrt(std[ctr]/200);
	
	}
	int Pctr = 0;
	cout << "------------------- Result -------------------" << endl; 
	cout << "Time Consumption : " << "  Average (ms) |  Std Dev (ms)  |" << endl;
	cout << "Capture an image : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "Show color image : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "Save color image : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "Define an image* : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "RGB Segmentation : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "HSV Segmentation : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "Show binary image: " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
	cout << "Overall Duration : " << average[Pctr] <<" | "<< std[Pctr] << endl; Pctr++;
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
	cout << "Capture an image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	//cv::Mat hsv;
   	//cv::cvtColor(cameraFrame, hsv, CV_BGR2HSV);
	start = std::chrono::high_resolution_clock::now();
	imshow("Camera",cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "Show a color image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	start = std::chrono::high_resolution_clock::now();
	imwrite( "./cameraImg.jpg", cameraFrame );
	finish = std::chrono::high_resolution_clock::now();
	cout << "Save a color image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	start = std::chrono::high_resolution_clock::now();
	Mat SegedRGBImg, SegedHSVImg;
	finish = std::chrono::high_resolution_clock::now();
	cout << "Define an image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	start = std::chrono::high_resolution_clock::now();
	SegedRGBImg = colorSegRGB(cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "RGB Segmentation: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	start = std::chrono::high_resolution_clock::now();
	SegedHSVImg = colorSegHSV(cameraFrame);
	finish = std::chrono::high_resolution_clock::now();
	cout << "HSV Segmentation: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	start = std::chrono::high_resolution_clock::now();
        imshow("Show a binary image",SegedRGBImg);
	finish = std::chrono::high_resolution_clock::now();
	cout << "Show a binary image: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000;

	

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
	cout << "Open Camera: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";

	Mat cameraFrame;
	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture First Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";

	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture Second Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";

	start = std::chrono::high_resolution_clock::now();
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
	finish = std::chrono::high_resolution_clock::now();
	cout << "-----------Capture third Frame: -----------------" << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/1000000 << "ms\n";


    while(Frame_ctr < 200) {
	cout << "------------- Next Frame: " << Frame_ctr << "--------------"<< endl;
	auto start1 = std::chrono::high_resolution_clock::now();
        eva_loop(camera);
	auto finish1 = std::chrono::high_resolution_clock::now();
	cout << "Overall: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish1-start1).count())/1000000 << "ms\n";
	timeMatrix[Frame_ctr][para_ctr++] = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish1-start1).count())/1000000;
        
	char keypress = cv::waitKey(20);
        if (keypress==27) {
            break;
        }
	Frame_ctr++;
	para_ctr = 0;
    }
	cout << "------------------- here? Before Entering cal_Meanstd() -------------------" << endl;

	calculate_MeanStd();

    return 0;
}
