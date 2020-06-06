/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erosion.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 09:28:37 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/03 09:28:37 by xiang chen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat srcPic = imread("../lena.jpg");
    imshow("src pic", srcPic);
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    //getStructuringElement函数返回的是指定形状和尺寸的结构元素
    Mat dstPic;
    erode(srcPic, dstPic, element); //腐蚀操作
    imshow("腐蚀效果", dstPic);
    waitKey();
    return 0;
}