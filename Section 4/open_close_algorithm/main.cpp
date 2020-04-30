#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main() {
    Mat image = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 4\\open_close_algorithm\\1.jpg");
    //specify the kernel size
    int kernelSize = 10;
    //create the kernel
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2*kernelSize+1, 2*kernelSize+1), Point(kernelSize,kernelSize));
    Mat imEroded;
    //perform erosion
    erode(image, imEroded, element, Point(-1,-1),1);
    Mat imOpen;
    //perform dilation
    dilate(imEroded, imOpen, element, Point(-1,-1),1);
    imshow("m1", imOpen);
    imwrite("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 4\\open_close_algorithm\\m1.png", imOpen);
    //API for open operation
    Mat imageMorphOpened;
    //morphologyEx(image, imageMorphOpened, MORPH_OPEN, element, Point(-1,-1), 3);
    morphologyEx(image, imageMorphOpened, MORPH_CLOSE, element, Point(-1,-1), 3);
    imshow("m2", imageMorphOpened);
    waitKey(0);
    return 0;
}
