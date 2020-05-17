#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat src = imread("../sample.jpg");
    imshow("input", src);
    
    //创建卷积核
    Mat k = Mat::ones(Size(5,5), CV_32FC1);

    //归一化
    k = k/25.0;
    //输出
    cout<<k << endl;

    //卷积操作
    Mat result;
    filter2D(src, result, -1, k, Point(-1,-1), 0, BORDER_DEFAULT);
    imshow("result", result);
    waitKey(0);
    return 0;
}