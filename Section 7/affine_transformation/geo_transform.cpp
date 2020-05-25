#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("../blue-square.png");
    imshow("input", image);
    namedWindow("result", WINDOW_AUTOSIZE);

    //平移到（25， 25）
    float warpMatValues[] = {1.0, 1.0, 25.0, 0.0, 1.0, 25.0};
    Mat warpMat = Mat(2, 3, CV_32F, warpMatValues);

    //warp image
    Mat result;
    Size outDim = image.size();
    warpAffine(image, result, warpMat, outDim);
    imshow("result", result);
    waitKey(0);

    //宽度放大两倍
    float warpMatVales2[] = {2.0, 0.0, 0.0, 0.0, 1.0, 0.0};
    Mat result1, warpMat1;
    warpMat1 = Mat(2, 3, CV_32F, warpMatVales2);
    warpAffine(image, result1, warpMat1, Size(outDim.width * 2, outDim.height));
    imshow("result1", result1);
    waitKey(0);

    //旋转图像
    Mat result2, warpMat2;
    float angleInRadius = 30;
    angleInRadius = 30 * 3.14 / 180.0;
    
    float cosTheta = cos(angleInRadius);
    float sinTheta = sin(angleInRadius);

    float warpMatValues3[] = {cosTheta, sinTheta, 0.0, -sinTheta, cosTheta, 0.0};
    warpMat2 = Mat(2, 3, CV_32F, warpMatValues3);
    warpAffine(image, result2, warpMat2, outDim);
    imshow("result2", result2);
    waitKey(0);
    return 0;
}