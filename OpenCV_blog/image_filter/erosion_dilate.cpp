#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    //dilate
    Mat img = imread("../lena.jpg");
    namedWindow("原始图", WINDOW_NORMAL);
    imshow("原始图", img);
    Mat out;
    //获取自定义核
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    //dilate
    dilate(img, out, element);
    namedWindow("膨胀操作", WINDOW_NORMAL);
    imshow("膨胀操作", out);
    waitKey(0);

    Mat out1;
    //获取自定义核
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    //腐蚀操作
    erode(img, out1, element);
    namedWindow("腐蚀操作", WINDOW_NORMAL);
    imshow("腐蚀操作", out1);
    waitKey(0);

    Mat out2;
    //获取自定义核
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    //高级形态学处理，调用这个函数就可以，具体操作选择哪种参数，就修改第三个参数即可。
    morphologyEx(img, out2, MORPH_GRADIENT, element);
    namedWindow("形态学处理操作", WINDOW_NORMAL);
    imshow("形态学处理操作", out);
    waitKey(0);
}