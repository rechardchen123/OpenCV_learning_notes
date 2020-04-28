#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main() {
    Mat img = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 4\\threshold\\lena.jpg");
    imshow("input", img);
    Mat gray,thre,thre1,thre2,thre3;
    cvtColor(img,gray, COLOR_BGR2GRAY);
    threshold(gray,thre,127,255,THRESH_BINARY_INV);
    imshow("thre",thre);
    //adaptive threshold value calculation 1
    double t = threshold(gray,thre1,127,255, THRESH_BINARY|THRESH_OTSU);
    double t1 = threshold(gray, thre2, 127,255,THRESH_BINARY|THRESH_TRIANGLE);
    adaptiveThreshold(gray, thre3, 255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25,10);
    cout<<"The adaptive threshold value is:"<<t<<endl;
    cout<<"The trangle adaptive threshold value is:"<<t1<<endl;
    imshow("adaptive threshold", thre1);
    imshow("trangle adaptive threshold",thre2);
    imshow("Gaussian threshold",thre3);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
