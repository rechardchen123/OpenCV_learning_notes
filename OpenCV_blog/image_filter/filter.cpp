/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/04 20:58:01 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/04 20:58:01 by xiang chen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("lol4.jpg");
    imshow("原始图", img);
    Mat out;
    boxFilter(img, out, -1, Size(5, 5)); //-1指原图深度
    imshow("方框滤波", out);
    waitKey(0);

    Mat out1;
    blur(img, out1, Size(5, 5)); //-1指原图深度
    imshow("均值滤波", out1);
    waitKey(0);

    Mat out2;
    GaussianBlur(img, out2, Size(3, 3), 0, 0);
    imshow("高斯滤波", out2);
    waitKey(0);

    Mat out3;
    medianBlur(img, out3, 7); //第三个参数表示孔径的线性尺寸，它的值必须是大于1的奇数
    imshow("中值滤波", out3);
    waitKey(0);

    Mat out4;
    bilateralFilter(img, out4, 25, 25 * 2, 25 / 2);
    imshow("双边滤波", out4);
    waitKey(0);
}