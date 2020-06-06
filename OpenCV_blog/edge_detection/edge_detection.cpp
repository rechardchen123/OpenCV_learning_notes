/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edge_detection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 21:29:35 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/04 21:29:35 by xiang chen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
/****************************************************************************** */
/*边缘检测的一般步骤:                                                            */
/*1. 滤波-消除噪声                                                              */
/*2. 增强-是边界轮廓更加凸显                                                     */
/*3. 检测- 选出边缘点                                                            */
/****************************************************************************** */

int main()
{
    Mat img = imread("../lena.jpg");
    imshow("原始图", img);

    Mat dstPic, edge, grayImage;
    //创建于src同类型的和大小的矩阵
    dstPic.create(img.size(), img.type());

    //原始图像转换为灰度图
    cvtColor(img, grayImage, COLOR_BGR2GRAY);

    //使用3*3内核来降噪
    blur(grayImage, edge, Size(3, 3));

    //运行canny检测算子
    Canny(edge, edge, 3, 9, 3);
    imshow("边缘提取效果", edge);
    waitKey(0);

    //Soble算法
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, dst;

    //求X方向的梯度
    Sobel(img, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x); //求取的梯度转换成绝对值
    imshow("x方向的sobel", abs_grad_x);

    //求y方向梯度
    Sobel(img, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);
    imshow("y向soble", abs_grad_y);

    //合并梯度
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    imshow("整体方向soble", dst);
    waitKey(0);

    //Laplacian算法
    Mat gray, dst, abs_dst;
    //高斯滤波消除噪声
    GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
    //转换成灰度图
    cvtColor(img, gray, COLOR_BGR2GRAY);
    //使用Laplace函数
    //第三个参数：目标图像深度；第四个参数：滤波器孔径尺寸；第五个参数：比例因子；第六个参数：表示结果存入目标图
    Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    //计算绝对值, 并转换结果
    convertScaleAbs(dst, abs_dst);
    imshow("laplace效果图", abs_dst);
    waitKey(0);
    return 0;
}