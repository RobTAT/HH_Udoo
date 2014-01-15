#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <cmath>
#include <fstream>

#define MAX_FRAME_NUMBER 200

#define ms 1000000
#define us 1000
#define ns 1
#define loop_ms 100*ms

#define ms_str "ms"
#define us_str "us"
#define ns_str "ns"

using namespace std;
using namespace cv;

int time_unit = us; // Modify unit of time here
string time_unit_string = us_str; // Also the string

Mat cameraFrame;

int minRGB[3] = {122,10,10};
int maxRGB[3] = {240,50,50};
int minHSV[3] = {0,175,159};
int maxHSV[3] = {180,255,250};
int objectcPos[2];

auto base_timeline_s = std::chrono::high_resolution_clock::now();
auto base_timeline_e = std::chrono::high_resolution_clock::now();

fstream file("/dev/ttymxc3");

void MyFilledCircle( Mat img, Point center )
{
 int thickness = -1;
 int lineType = 8;

 circle( img,
         center,
         800/200.0,
         Scalar( 0, 255, 0 ),
         thickness,
         lineType );
}

Point find_Center(Mat src){
        
        int img_diagram_x [src.cols];
        int img_diagram_y [src.rows];

        int x_start = 0, y_start = 0, x_end = 0, y_end = 0;
        cout << "======================= New Frame ========================== " << endl;
        //cout << "channel: " << src.channels() << endl;
        //cout << "image type" << src.type() << endl;
        //cout << "size of char" << sizeof(char) << endl ;
        
        for(int i=0; i <= src.cols; i++){
                img_diagram_x [i] = 0;
        }

        for(int i=0; i <= src.rows; i++){
                img_diagram_y [i] = 0;
        }        


        for(int i=0; i <= src.rows; i++){
                for(int j=0; j<= src.cols; j++){
                        if (((int)src.at<uchar>(i,j))>20){
                                img_diagram_x [j] += 1;
                                img_diagram_y [i] += 1;
                                //cout << "Pixel at "<< i << "," << j << " " << " are Blah, Value are " << (int)src.at<uchar>(i,j) << endl;
                        }        

                }
        }

/*        
        for(int i=0; i <= src.rows; i++){
                cout << "rows val " << i << ": " << img_diagram_x [i] << endl;
        }

        for(int i=0; i <= src.cols; i++){
                cout << "cols val " << i << ": " << img_diagram_y [i] << endl;
        }
*/

        for(int i=0; i <= src.cols; i++){
                if ((img_diagram_x [i] > 15)&&(img_diagram_x [i+1] > 15)&&(img_diagram_x [i+2] > 15)&&(img_diagram_x [i+3] > 15)&&(img_diagram_x [i+4] > 15)) {
                        x_start = i;
                        break;
                }
        }

        for(int i=src.cols; i >= x_start; i--){
                if ((img_diagram_x [i] > 15)&&(img_diagram_x [i+1] > 15)&&(img_diagram_x [i+2] > 15)&&(img_diagram_x [i+3] > 15)&&(img_diagram_x [i+4] > 15)) {
                        x_end = i;
                        break;
                }
        }

        for(int j=0; j <= src.rows; j++){
                if ((img_diagram_y [j] > 15)&&(img_diagram_y [j+1] > 15)&&(img_diagram_y [j+2] > 15)&&(img_diagram_y [j+3] > 15)&&(img_diagram_y [j+4] > 15)) {
                        y_start = j;
                        break;
                }
        }

        for(int j=src.rows; j >= y_start; j--){
                if ((img_diagram_y [j] > 15)&&(img_diagram_y [j+1] > 15)&&(img_diagram_y [j+2] > 15)&&(img_diagram_y [j+3] > 15)&&(img_diagram_y [j+4] > 15)) {
                        y_end = j;
                        break;
                }
        }
        
        cout << "xe: " << x_end << " xs " << x_start << " , ye: " << y_end << " ys " << y_start << endl;
        cout << "center: " << ((x_end + x_start)/2) << " , " <<  ((y_end + y_start)/2) << endl;

        int center_x = (x_end + x_start)/2;
        int center_y = (y_end + y_start)/2;

        if( center_x >= 320){
		fstream file("/dev/ttymxc3");
                file << "r" ;
		cout << "msg sent: r" << endl;
        }        else if( center_x < 320) {
		fstream file("/dev/ttymxc3");
                file << "l" ;
		cout << "msg sent: l" << endl;
        }

        return Point(center_x,center_y);
        
}

Mat closeOperation(Mat src){
        Mat erosion_dst, dilation_dst, duo;
        int erosion_type, dilation_type, erosion_size, dilation_size;
        erosion_type = MORPH_ELLIPSE;
        dilation_type = MORPH_ELLIPSE;
        erosion_size = 1;
        dilation_size = 1;

        Mat element_erosion = getStructuringElement( erosion_type,
                                                            Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                                            Point( erosion_size, erosion_size ) );

        Mat element_dilation = getStructuringElement( dilation_type,
                                                            Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                            Point( dilation_size, dilation_size ) );
        erode( src, erosion_dst, element_erosion );
        dilate( src, dilation_dst, element_dilation );
        dilate( erosion_dst, duo, element_dilation );

        //imshow( "erosion Demo", erosion_dst );
        //imshow( "dilation Demo", dilation_dst );
        //imshow( "Duo", duo );
        
        return duo;
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
        cvtColor(src, HSV_Image, CV_BGR2HSV);
        inRange(HSV_Image, Scalar(minHSV[0], minHSV[1], minHSV[2]), 
                     Scalar(maxHSV[0], maxHSV[1], maxHSV[2]), filtered_Image);
        return filtered_Image;
}

void img_proc(VideoCapture camera){

        Mat cameraFrame;
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }

        Mat SegedHSVImg, SegedRGBImg, BlurImg, GrayImg;

        auto start = std::chrono::high_resolution_clock::now();
        //SegedHSVImg = colorSegHSV(cameraFrame);
        SegedRGBImg = colorSegRGB(cameraFrame);

        auto finish = std::chrono::high_resolution_clock::now();
        cout << "RGB Segmentation: " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count())/time_unit << " "<< time_unit_string << endl;

        //imshow("HSV_filtered_Image", SegedHSVImg);
        
        //GaussianBlur(SegedRGBImg, BlurImg, Size( 51, 51 ), 0, 0 );
        

        //imshow("RGB_filtered_Image", SegedRGBImg);
        //imwrite("./image.jpg", cameraFrame);
        //imshow("filtered", closeOperation(SegedRGBImg));

        SegedRGBImg = closeOperation(SegedRGBImg);
        
        Point object_Center = find_Center(SegedRGBImg);

        //Mat centerImg = Mat::zeros( SegedRGBImg.rows, SegedRGBImg.cols, CV_8UC3 );//

        //cout << "!! Center: " << object_Center.x << " , " << object_Center.y << endl;
        
        MyFilledCircle( cameraFrame, object_Center);

        imshow("SegedRGBImg", SegedRGBImg);
        imshow("src", cameraFrame);
}


int main(int argc, char** argv) {
    
    int cameraNumber = 0;
    int time_Interval;

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

        // Skip 3 frame:
        camera>>cameraFrame;
        camera>>cameraFrame;
        camera>>cameraFrame;
        if (cameraFrame.empty()){
            std::cerr<<"No frame read from camera"<<std::endl;
            exit(1);
        }
        

    while(true) {

        base_timeline_s = std::chrono::high_resolution_clock::now();

        img_proc(camera); // add your image process code here
        
        char keypress = cv::waitKey(20);
        if (keypress==27) {
            break;
        }

        base_timeline_e = std::chrono::high_resolution_clock::now();
        while (((float)(std::chrono::duration_cast<std::chrono::nanoseconds>(base_timeline_e - base_timeline_s).count()))<=(float)(200000000)){
                base_timeline_e = std::chrono::high_resolution_clock::now();
        }
        time_Interval = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(base_timeline_e-base_timeline_s).count())/((float)ms);

        if (time_Interval >= 101*((float)ms)){
                cout << "=========== [Error] timeline Interval Exceed 100ms : " << time_Interval << "=================" << endl;
        }

	//std::string response;
	//file >> response;
    	//std::cout << "Response: " << response << std::endl;

    }

    return 0;
}
