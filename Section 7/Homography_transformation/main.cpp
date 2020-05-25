#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //read the images
    Mat im_src = imread("../book2.jpg");
    imshow("input1", im_src);
    //four corners of the book in source image
    vector<Point2f> pts_src{Point2f(141, 131), Point2f(480, 159), Point2f(493, 630), Point2f(64, 601)};

    //read the destination image
    Mat im_dst = imread("../book1.jpg");
    imshow("input2", im_dst);
    vector<Point2f> pts_dst{Point2f(318, 256), Point2f(534, 372), Point2f(316, 670), Point2f(73, 473)};

    //calculate homography
    Mat h = findHomography(pts_src, pts_dst);

    //warp the source image to destination based on homography
    Mat im_out;
    warpPerspective(im_src, im_out, h, im_dst.size());
    imshow("output", im_out);
    waitKey(0);

    return 0;
}
