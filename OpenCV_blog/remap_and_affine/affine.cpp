#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat src = imread("../2.jpg");
    Mat dst_warp, dst_warpRotateScale;
    Point2f srcPoints[3]; // 原图中的三点
    Point2f dstPoints[3]; //目标图中的三点 

    //第一种仿射变换的调用方式：三点法
	//三个点对的值,上面也说了，只要知道你想要变换后图的三个点的坐标，就可以实现仿射变换 
    srcPoints[0] = Point2f(0,0);
    srcPoints[1] = Point2f(0, src.rows - 1);
    srcPoints[2] = Point2f(src.cols - 1, 0);
    //映射后的三个坐标值
    dstPoints[0] = Point2f(0, src.rows*0.3);
    dstPoints[1] = Point2f(src.cols*0.25, src.rows*0.75);
	dstPoints[2] = Point2f(src.cols*0.75, src.rows*0.25);

    Mat m1 = getAffineTransform(srcPoints, dstPoints); //由三个点对计算变换矩阵  
    warpAffine(src, dst_warp, m1, src.size()); //仿射变换  

    //第二种仿射变换的调用方式：直接指定角度和比例										  
	//旋转加缩放  
    Point2f center(src.cols /2, src.rows /2); //rotate center
    double angle = 45;
    double scale = 0.5;

    Mat m2 = getRotationMatrix2D(center, angle, scale);
    warpAffine(dst_warp, dst_warpRotateScale, m2, src.size());

    imshow("原始图", src);
	imshow("仿射变换1", dst_warp);
	imshow("仿射变换2", dst_warpRotateScale);

    waitKey(0);
    return 0;
}