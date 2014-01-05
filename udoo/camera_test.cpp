#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;

// RGB color segmentation
Mat colorSegRGB(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat binaryImg;
    inRange(src, Scalar(50, 59, 23), Scalar(180, 150, 150), binaryImg);
    return binaryImg;
}

int main(int argc, char** argv) {

    int cameraNumber = 3;
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

    while(true) {
        Mat cameraFrame;
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }

        // Show image
        imshow("Camera",cameraFrame);
        imwrite( "./cameraImg.jpg", cameraFrame );
        imshow("Filtered Image",colorSegRGB(cameraFrame));

        char keypress = cv::waitKey(20);
        if (keypress==27) {
            break;
        }
    }
    return 0;
}
