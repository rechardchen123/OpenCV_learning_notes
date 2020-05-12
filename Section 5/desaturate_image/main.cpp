#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
int main() {
    Mat img = imread("../girl.jpg");
    imshow("input", img);

    //convert to HSV color space
    Mat hsvImage;
    cvtColor(img, hsvImage, COLOR_BGR2HSV);
    //conver to float32
    hsvImage.convertTo(hsvImage, CV_32F);
    vector<Mat> channels(3);
    //split the channels
    split(hsvImage, channels);
    Mat S = channels[1] * 0.02; //the value of 0.02 is a saturation scale value.
    //clipping operation performed to limit pixel values between 0 and 255
    channels[1] = S;
    merge(channels, hsvImage);
    //convert back from float32
    hsvImage.convertTo(hsvImage, CV_8UC3);
    Mat dst;
    cvtColor(hsvImage, dst, COLOR_HSV2BGR);
    imshow("result", dst);
    waitKey(0);
    return 0;
}
