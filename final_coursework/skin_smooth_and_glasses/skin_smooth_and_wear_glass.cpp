/*********************************************************************************
*FileName:  // skin_makeup 
*Author:  //richard_chen
*Version:  //1.0
*Date:  //2020-06-13
*Description: // 1. open the camera
              // 2. detect the face
              // 3. display the image
**********************************************************************************/

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

CascadeClassifier faceDetector;
String face_cascade = "../haarcascade_frontalface_alt.xml";

void wearMask(Mat &face, Mat &glasses, Mat &mask)
{
    Rect rect;
    rect.x = 0;
    rect.y = face.rows * 0.3;
    rect.width = face.cols;
    rect.height = face.rows * 0.3;
    Mat temp1, temp2;
    resize(glasses, temp1, Size(rect.width, rect.height));
    resize(mask, temp2, Size(rect.width, rect.height));
    Mat temp3 = face(rect);
    temp1.copyTo(temp3, temp2);
}

void smoothFace(Mat &face)
{
    Mat temp = face.clone();
    bilateralFilter(temp, face, 5, 100, 10);
    temp.release();
}

/*Main function */
int main(int argc, char **argv)
{
    //read the sunglass
    Mat glass = imread("../glass_image.jpg");
    Mat mask, gray1, binary;
    GaussianBlur(glass, mask, Size(3,3),0,0);
    cvtColor(mask, gray1, COLOR_BGR2GRAY);
    threshold(gray1, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    for (size_t t = 0; t < contours.size(); t++) {
		drawContours(mask, contours, t, Scalar(0, 0, 255), 2, 8);
	}
    imshow("mask", mask);
    //read the video from the device
    VideoCapture cap(0);
    Mat frame, gray;
    vector<Rect> faces;

    //load the face and eye detector
    faceDetector.load(face_cascade);

    while (1)
    {
        cap >> frame;
        //smooth the face
        smoothFace(frame);
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        //detect the face from the video
        faceDetector.detectMultiScale(gray, faces, 1.2, 1, 0, Size(30, 30), Size(400, 400));
        for (size_t i = 0; i < faces.size(); i++)
        {
            wearMask(frame, glass, mask);
            rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8, 0);
        }

        char c = waitKey(10);
        if (c == 27)
            break;

        imshow("OpenCV-face makeup", frame);
    }
    waitKey(0);
    return 0;
}