#include<opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char** argv){
	
	const char* filename = argc >= 2? argv[1] : "p1.jpg";

	Mat input_Image = imread(filename, 1);

    	imshow("opencvtest",input_Image);
    	waitKey(0);

   	return 0;
}
