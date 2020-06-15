#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class FaceMakeUp{
    public:
        FaceMakeUp();
        void alginFace(Mat &face);
        void smoothSkin(Mat &face);
        void wearMask(Mat &face, Mat &glasses, Mat &mask, vector<Point>landmarks);
        void lipstick(Mat &face, Scalar color, vector<Point>landmarks);
};

FaceMakeUp::FaceMakeUp(){
    //empty
}

void FaceMakeUp::alginFace(Mat &face){

}

void FaceMakeUp::smoothSkin(Mat &face){
    Mat temp = face.clone();
    cv::bilateralFilter(temp,  face, 5, 100, 10);
    temp.release();
}

void FaceMakeUp::wearMask(Mat &face, Mat &glasses, Mat &mask, vector<Point>landmarks){
    float edx = landmarks[34].x - landmarks[18].x;
    circle(face, landmarks[18], 2, Scalar(0, 0, 255), 2, 8, 0);
    float dx = mask.cols;
    float rate = edx /dx;
    cout<< "rate: %.2f"<<rate;
    Mat t1, t2;
    resize(glasses, t1, Size(0, 0), rate, rate);
    resize(mask, t2, Size(0,0), rate, rate);
    int w = t2.cols;
    int h = t2.rows;
    int x = landmarks[18].x;
    int y = landmarks[18].y - (t2.rows/2);
    Rect rect(x, y, w, h);
    Mat roi = face(rect);
    t1.copyTo(roi, t2);
    t1.release();
    t2.release();
}