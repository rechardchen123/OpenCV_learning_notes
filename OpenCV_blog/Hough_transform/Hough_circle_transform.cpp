#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat srcImage = imread("../2.jpg");
    Mat midImage, dstImage;

    imshow("origin image", srcImage);

    //transform into gray image and smooth the image
    cvtColor(srcImage, midImage, COLOR_BGR2GRAY);
    GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);

    //Hough circle transform
    vector<Vec3f> circles;
    //注意第七的参数为阈值，可以自行调整，值越大，检测的圆更精准
    HoughCircles(midImage, circles, HOUGH_GRADIENT, 1.5, 10, 200, 150, 0, 0);

    //draw the circle
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        //draw the circle center
        circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        //draw the circle edge
        circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
    }
    imshow("effective image", srcImage);
    waitKey(0);
    return 0;
}