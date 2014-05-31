#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

Vec3b averagePix( Mat &image );
void drawObject(int x, int y, Mat &image);
void pixAverage(Mat &image, int &R, int &B, int &G);
Mat detectSkin(Mat &image);

const int FRAME_WIDTH = 640; //#cols
const int FRAME_HEIGHT = 480; //#rows

int main(){

	Mat image;
	Mat skin;
	Vec3b pixVec;	// average BGR of the image
	int key;		//keypress ASCII value

	VideoCapture cap;	//Videocapture object to acquire webcam feed
	cap.open(0);
	if(!cap.isOpened()){
		cout << "Unable to start video feed. Check webcam. " << endl;
		return -1;
	}

	// create window to show image
	namedWindow("Window",1);

	int R = 0, G = 0, B = 0;

	while(1){
		
		//copy webcam stream to image
		cap.read(image);
		drawObject(100, 150, image);
		skin = detectSkin(image);

		imshow("Window", skin);

		pixAverage(image,R,G,B);
		cout << "R = " << R <<"\t G = " << G << "\t B = " << B << endl;

		// wait 33ms between frames, quit if user presses ESC
		key = waitKey(33);
		if (key == 27) {
			break;
		}
	}
	
	destroyAllWindows();
	cap.release();

	return 0;
}

//draw crosshair on top of image
void drawObject(int x, int y, Mat &image){
	int lineWidth = 1;
	int size = 20;
	stringstream ss_x, ss_y;

	circle(image, Point(x,y), 10, Scalar(0, 255, 0), 1);
	if(y-size>0)
    line(image,Point(x,y),Point(x,y-25),Scalar(0,255,0),lineWidth);
    else line(image,Point(x,y),Point(x,0),Scalar(0,255,0),lineWidth);
    if(y+size<FRAME_HEIGHT)
    line(image,Point(x,y),Point(x,y+25),Scalar(0,255,0),lineWidth);
    else line(image,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),lineWidth);
    if(x-size>0)
    line(image,Point(x,y),Point(x-25,y),Scalar(0,255,0),lineWidth);
    else line(image,Point(x,y),Point(0,y),Scalar(0,255,0),lineWidth);
    if(x+size<FRAME_WIDTH)
    line(image,Point(x,y),Point(x+25,y),Scalar(0,255,0),lineWidth);
    else line(image,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),lineWidth);

	ss_x << x;
	ss_y << y;
	putText(image,ss_x.str()+","+ss_y.str(),Point(x,y+30),1,1,Scalar(0,255,0),lineWidth);
}



// detect skin in image. Must convert to YCbCr space
Mat detectSkin(Mat &image){
	//thresholds: (Cb>=77 & Cb<=127 & Cr>=133 & Cr<=173)
	Mat output;
	Mat YCbCr;
	Mat Cb_mask;
	Mat Cr_mask;
	Mat channels[3];	//contains individual channels

	cvtColor(image, YCbCr, CV_BGR2YCrCb);
	split(YCbCr, channels);
	inRange(channels[1], 77, 127, Cb_mask); 
	inRange(channels[2], 133, 173, Cr_mask);
	bitwise_and(Cb_mask, Cr_mask, output);
	return output;
}


//compute average BGR values
void pixAverage(Mat &image, int &R, int &B, int &G){
		for(int i=0; i<image.rows; i++){
		for(int j=0; j<image.cols; j++){
			B += image.at<Vec3b>(i,j)[0];
			G += image.at<Vec3b>(i,j)[1];
			R += image.at<Vec3b>(i,j)[2];
		}
	}
	B = (int) B/(image.rows*image.cols);
	G = (int) G/(image.rows*image.cols);
	R = (int) R/(image.rows*image.cols);
}