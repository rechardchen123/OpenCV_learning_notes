#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat src = imread("../sample.jpg");
    imshow("input", src);
    Mat edges, dst;
    int t1 =100;
    Canny(src, edges, t1, t1*3);
    bitwise_and(src, src, dst, edges);
    imshow("canny", dst);

    waitKey(0);
    return 0;
}