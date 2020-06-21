#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat g_srcImage, g_srcImage1, g_grayImage;
int thresh = 30;
int max_thresh = 175;

void on_CornerHarris(int, void *); 

int main(int argc, char ** argv){
    g_srcImage = imread("../1.png", 1);
    if(!g_srcImage.data){
        cout<< "reading image error!"<<endl;
        return -1;
    }
    imshow("原始图", g_srcImage);
    g_srcImage1 = g_srcImage.clone();

    //存留一张灰度图  
	cvtColor(g_srcImage1, g_grayImage, COLOR_BGR2GRAY);

    //创建窗口和滚动条  
	namedWindow("角点检测", WINDOW_AUTOSIZE);
	createTrackbar("阈值: ", "角点检测", &thresh, max_thresh, on_CornerHarris);

    //调用一次回调函数，进行初始化  
	on_CornerHarris(0, 0);

    waitKey(0);
    return 0;
}

void on_CornerHarris(int, void*){
    Mat dstImage;
    Mat normImage; //归一化后的图
    Mat scaledImage; //线性变换后的八位无符号整型的图  

    //置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值  
    dstImage = Mat::zeros(g_srcImage.size(), CV_32FC1);
    g_srcImage1 = g_srcImage.clone();

    //进行角点检测  
	//第三个参数表示邻域大小，第四个参数表示Sobel算子孔径大小，第五个参数表示Harris参数
    cornerHarris(g_grayImage, dstImage, 2, 3, 0.04, BORDER_DEFAULT);

    // 归一化与转换  
	normalize(dstImage, normImage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(normImage, scaledImage);//将归一化后的图线性变换成8位无符号整型  

    // 将检测到的，且符合阈值条件的角点绘制出来  
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			//Mat::at<float>(j,i)获取像素值，并与阈值比较
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				circle(g_srcImage1, Point(i, j), 5, Scalar(10, 10, 255), 2, 8, 0);
				circle(scaledImage, Point(i, j), 5, Scalar(0, 10, 255), 2, 8, 0);
			}
		}
	}

    imshow("角点检测", g_srcImage1);
	imshow("角点检测2", scaledImage);

}