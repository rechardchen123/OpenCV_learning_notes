#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main() {
    string imagePath = "C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding"
                       "\\OpenCV_learning_notes\\Section 4\\contour\\Contour.png";
    Mat image = imread(imagePath);
    imshow("input", image);

    Mat imageGray;
    cvtColor(image, imageGray, COLOR_BGR2GRAY);

    //contour find
    vector<vector<Point>>contours; //轮廓的点集
    vector<Vec4i>hierarchy; //层次信息, 类型为int
    findContours(imageGray, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
    cout<<"Number of contours found = "<< contours.size();

    //contour drawing
    Mat drawing = image.clone(); //在原图上绘制轮廓
    drawContours(drawing, contours, -1, Scalar(0,0,255),2,8); //绘制轮廓
    Moments M;
    int x,y;
    double area;
    double perimeter;
    for (size_t i = 0; i<contours.size();i++){
        M = moments(contours[i]);
        x = int(M.m10/double(M.m00));
        y = int(M.m01/double(M.m00));
        //mark the center
        circle(drawing, Point(x,y), 10, Scalar(255,0,0),-1);
        //calculate the area and perimeter
        area = contourArea(contours[i]);
        perimeter = arcLength(contours[i], true);
        cout<< "Contour #"<<i +1<<"has area = "<< area << " and perimeter = "<< perimeter <<endl;
    }
    imshow("findContours-demo", drawing);

    Mat image1 = drawing.clone();
    Rect rect;
    RotatedRect rotrect;
    Point2f rect_points[4];
    Mat boxPoints2f, boxPointsCov;
    for(size_t i = 0; i< contours.size();i++){
        //vertical rectangle
        rect = boundingRect(contours[i]);
        rectangle(image1, rect, Scalar(255,0,255),2);
        //Rotated rectangle
        rotrect = minAreaRect(contours[i]);
        boxPoints(rotrect, boxPoints2f);
        boxPoints2f.assignTo(boxPointsCov, CV_32S);
        polylines(image1, boxPointsCov, true, Scalar(0,255,255),2);
    }
    imshow("rect", image1);

    // circle and ellipse
    Mat image2 = drawing.clone();
    Point2f center;
    float radius;
    RotatedRect rellipse;
    for(size_t i = 0; i < contours.size();i++){
        //fit a circle
        minEnclosingCircle(contours[i], center, radius);
        circle(image2, center, radius, Scalar(125,125,125),2);
        //ellipse
        if(contours[i].size()<5)
            continue;
        rellipse = fitEllipse(contours[i]);
        ellipse(image, rellipse, Scalar(255,0,125),2);
    }
    imshow("circle", image2);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
