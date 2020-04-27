#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point center, circumference;
Mat source;
void drawCircle(int action, int x, int y, int flags, void *userdata){
    //action to be taken when left mouse button is pressed
    if(action == EVENT_LBUTTONDOWN){
        center = Point(x,y);
        //Mark the center
        circle(source, center, 1, Scalar(255,255,0),2,LINE_AA);
    }
    //action to be taken when left mouse button is released
    else if (action == EVENT_LBUTTONUP){
        circumference = Point(x,y);
        //calculate raduis of the circle
        float radius = sqrt(pow(center.x- circumference.x,2)+pow(center.y-circumference.y,2));
        //draw the circle
        circle(source, center, radius,Scalar(0,255,0),2,LINE_AA);
        imshow("window", source);
    }
}
int main() {
    source = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\mouse_move\\lena.jpg");
    Mat dummy = source.clone();
    namedWindow("Window");
    //注册鼠标回调事件响应函数
    setMouseCallback("Window", drawCircle);
    int k = 0;
    while (k!=27){
        imshow("Window", source);
        putText(source, "choose center and drag, Press ESC to exit and c to clear", Point(10,30), FONT_HERSHEY_SIMPLEX,0.7, Scalar(255, 255, 255), 2);
        k = waitKey(20) & 0xFF;
        if (k == 99)
            // Another way of cloning
            dummy.copyTo(source);
    }
    return 0;
}
