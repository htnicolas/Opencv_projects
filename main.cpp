#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

Vec3b averagePix( Mat &image );

int main(){

	Mat image;
	
	Vec3b pixVec;	// average BGR of the image
	int key;

	VideoCapture cap;	//Videocapture object to acquire webcam feed
	cap.open(0);

	// create window to show image
	namedWindow("Window",1);

	while(1){
		//copy webcam stream to image
		cap>>image;
		imshow("window", image);

		pixVec = averagePix( image );
		cout << pixVec << endl << endl;

		// wait 33ms between frames, quit if user presses ESC
		key = waitKey(33);
		if (key == 27) {
			break;
		}
	}

	destroyAllWindows();

	return 0;
}

/* function to compute mean BGR value
input:  address of Mat image
output: 3x1 Vec3b vector
*/
Vec3b averagePix( Mat &image ){
	//3x1 vector
	Vec3b meanPix;	//Problem here: need to initialize meanPix to 0,0,0
	for( int i = 0; i < image.rows; i++ ){
		for( int j = 0; j < image.cols; j++ ){
			meanPix += image.at<Vec3b>(i,j);	//BGR
		}
	}
	return meanPix/(image.rows * image.cols);	//divide Vec3b by scalar?
}