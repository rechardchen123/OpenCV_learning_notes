#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //box convolution
    Mat image = imread("../sample.jpg");
    imshow("input", image);

    Mat dst1, dst2;
    // box filter - kernel size 3
    blur(image, dst1, Size(3, 3), Point(-1, -1));
    blur(image, dst2, Size(7, 7), Point(-1, -1));

    imshow("3X3 blur", dst1);
    imshow("7X7 blur", dst2);

    //Gaussian blur
    Mat dst3;
    GaussianBlur(image, dst3, Size(5, 5), 15, 0, 4);
    imshow("Gaussian blur", dst3);

    //median blur
    Mat dst4;
    int kernelsize = 5;
    medianBlur(image, dst4, kernelsize);
    imshow("Median blur", dst4);

    //bilateral filter
    Mat src = imread("../salt-and-pepper.png");
    Mat dst5;
    imshow("input1", src);
    bilateralFilter(src, dst5, 0, 100, 10);
    imshow("bilateral filter", dst5);


    waitKey(0);
    return 0;
}