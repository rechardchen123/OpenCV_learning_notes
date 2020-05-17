#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat src = imread("../sample.jpg");
    imshow("input", src);

    Mat sobel_x, sobel_y;
    Sobel(src, sobel_x, CV_32F, 1, 0,3);
    Sobel(src, sobel_y, CV_32F, 0,1,3);
    normalize(sobel_x, sobel_x, 0, 1, NORM_MINMAX);
    normalize(sobel_y, sobel_y, 0, 1, NORM_MINMAX);
    imshow("sobel_x", sobel_x);
    imshow("sobel_y", sobel_y);

    Mat dst;
    Laplacian(src, dst, CV_32F, 3);
    normalize(dst, dst, 0, 1, NORM_MINMAX);
    imshow("laplacian", dst);
    waitKey(0);
    return 0;
}