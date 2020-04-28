#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {
    Mat img = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 4\\corrosion_expansion\\lena.jpg");
    imshow("origin image", img);
    //erosion operation
    Mat dst;
    Mat se = getStructuringElement(MORPH_RECT,Size(5,5),Point(-1,-1));//get the construct element
    erode(img, dst, se);
    imshow("erosion", dst);
    // expansion operation
    Mat dst1;
    dilate(img, dst1, se);
    imshow("dilate", dst1);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
