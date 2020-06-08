#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

#define PIC_BEGIN_NUM 100  //这里定义你的起始图片编号
#define ANGLE_START -45//旋转角度的开始
#define ANGLE_END  0  //旋转角度的结束 
#define ANGLE_STEP 2 //旋转角度步长

int main(){
    //read a single channel image
    const char* filename = "../2.jpg";
    Mat srcImg  = imread(filename, 1);
    imshow("source", srcImg);
    Point center(srcImg.cols/2, srcImg.rows/2);
    char file[20];
    int count = PIC_BEGIN_NUM;
    Mat tmpImg;
    for(int tmp = ANGLE_START; tmp<ANGLE_END; tmp+=ANGLE_STEP){
        Mat rotMats = getRotationMatrix2D(center, tmp, 0.5);//图片缩小到原来的0.5倍
        warpAffine(srcImg, tmpImg, rotMats, srcImg.size(), 1, 0, Scalar(0,0,0));

        sprintf(file, "../%d.jpg", count++);
        imwrite(file, tmpImg);
    }

    waitKey(0);
    return 0;
}