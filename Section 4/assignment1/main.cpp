#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void assignment_1(){
    //read the image
    Mat image = imread("../CoinsA.png");
    imshow("Original image", image);
    // threshold operation
    Mat imageGray, binaryImage;
    cvtColor(image, imageGray, COLOR_BGR2GRAY); //transfer into gray image
    vector<Mat> mv;
    split(image, mv); // the color image has three channels, using the 'split' function to divide the three channels.
    threshold(mv[1], binaryImage, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);

    // morphology operation
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(binaryImage, binaryImage, MORPH_CLOSE, element);
    morphologyEx(binaryImage, binaryImage, MORPH_OPEN, element, Point(-1,-1),4);

    //contour finding
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(binaryImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());

    //contour analysis
    Mat drawing  = image.clone();
    Point2f center;
    float radius;
    Moments M;
    double x,y;
    for(size_t i=0; i< contours.size(); i++) {
        minEnclosingCircle(contours[i], center, radius);
        circle(drawing, center, radius, Scalar(0,0,255),2,8,0);
        M = moments(contours[i]);
        x = M.m10/double(M.m00);
        y = M.m01/double(M.m00);
        circle(drawing, Point(x,y), 2, Scalar(255,0,0),2,8,0);
    }
    imshow("binary image", binaryImage);
    imshow("find the contour of the coins", drawing);
    imwrite( "../result1.png", drawing);
    waitKey(0);
}

void assignment_2(){
    Mat image2 = imread("../CoinsB.png");
    imshow("Original Image2", image2);
    // threshold operation
    Mat gray, binary;
    cvtColor(image2, gray, COLOR_BGR2GRAY);
    threshold(gray, binary,0,255, THRESH_BINARY|THRESH_OTSU);
    imshow("binary", binary);
    imwrite("../binary2.png", binary);

    //morphology operation
    Mat se = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(binary, binary, MORPH_OPEN, se, Point(-1, -1));

    // contour finding
    vector<Vec4i> hierarchy;
    vector<vector<Point>> contour;
    bitwise_not(binary, binary);
    findContours(binary, contour, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
    Mat result = image2.clone();
    float radius;
    double area;
    for(size_t t = 0; t<contour.size();t++){
        area = contourArea(contour[t]);
        cout<<"Contour # "<< t+1<<" has area = " <<area<<endl;
        if(area < 1000)
            continue;
        RotatedRect rrt = fitEllipse(contour[t]);
        radius = min(rrt.size.width, rrt.size.height)/2.0;
        circle(result, rrt.center, radius, Scalar(0,0,255),4,8,0);
        Moments M = moments(contour[t]);
        double cx = M.m10/M.m00;
        double cy = M.m01/M.m00;
        circle(result, Point(cx, cy),2, Scalar(255,0,0),2,8,0);
    }
    imshow("result", result);
    imwrite("../result2.png", result);
    waitKey(0);
}

int main() {
    assignment_1();
    assignment_2();
    return 0;
}
