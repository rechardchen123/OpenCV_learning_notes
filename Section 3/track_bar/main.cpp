#include <iostream>
#include <opencv2/opencv.hpp>
using namespace  std;
using namespace cv;

int maxScaleUp= 100;
int scaleFactor = 1;
int scaleType = 0;
int maxType = 1;
Mat im;

string windowName = "Resize Image";
string trackbarValue = "Scale";
string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";


void scaleImage(int, void*);
int main() {
    //load the image
    im = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\track_bar\\lena.jpg");
    //build the window
    namedWindow(windowName, WINDOW_AUTOSIZE);
    //build the trackbar
    createTrackbar(trackbarValue,windowName, &scaleFactor, maxScaleUp, scaleImage);
    createTrackbar(trackbarType, windowName,&scaleType,maxType, scaleImage);
    scaleImage(25, 0);
    while(true){
        int c;
        c = waitKey(20);
        if(static_cast<char>(c) == 27)
            break;
    }
    return 0;
}

void scaleImage(int, void*){
    //get the scale factor from the trackbar
    double scaleFactorDouble = 1 + scaleFactor/100.0;
    if(scaleFactorDouble == 0){
        scaleFactorDouble = 1;
    }
    Mat scaledImage;
    //resize the image
    resize(im, scaledImage, Size(), scaleFactorDouble, scaleFactorDouble, INTER_LINEAR);
    imshow(windowName, scaledImage);
}