/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canny.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 09:56:16 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/03 09:56:16 by xiang chen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat srcPic = imread("../lena.jpg");
    imshow("Src Pic", srcPic);
    Mat DstPic, edge, grayImage;

    //创建与src同类型和大小的矩阵
    DstPic.create(srcPic.size(), srcPic.type());

    //将原始图像转化为灰度图像
    cvtColor(srcPic, grayImage, COLOR_BGR2GRAY);

    //先使用3X3的矩阵降噪
    blur(grayImage, edge, Size(3, 3));

    //运行canny算子
    Canny(edge, edge, 3, 9, 3);

    imshow("边缘提取效果", edge);
    waitKey();
    return 0;
}