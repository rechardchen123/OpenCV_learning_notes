/*
 * @Author: xiang chen
 * @Date: 2020-06-03 08:50:06
 * @LastEditTime: 2020-06-03 09:05:31
 * @LastEditors: Please set LastEditors
 * @Description: image binary opeator using OpenCV
 * @FilePath: \OpenCV_blog\image_binary.cpp
 */

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat image = imread("../lena.jpg", IMREAD_GRAYSCALE); //读入图像的时候载入灰度图的命令和opencv3不一样
    if (image.empty())
    {
        cout << "read image failure" << endl;
        return -1;
    }
    //全局二值化
    int th = 100;
    Mat global;
    threshold(image, global, th, 255, THRESH_BINARY);

    //局部二值化
    int blockSize = 25;
    int constValue = 10;
    Mat local;
    adaptiveThreshold(image, local, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, blockSize, constValue);

    imshow("全局二值化", global);
    imshow("局部二值化", local);

    waitKey(0);
    return 0;
}
