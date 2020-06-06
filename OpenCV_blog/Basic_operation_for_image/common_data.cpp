/* ************************************************************************** */
/*  Common data structure in OpenCV                                           */
/*                                                        :::      ::::::::   */
/*   common_data_image.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 10:01:27 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/03 10:01:27 by xiang chen       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    //Mat 用法
    Mat m1(2, 2, CV_8UC3, Scalar(0, 0, 255)); //宏的解释：cv_[位数][带符号与否][类型前缀]C[通道数]
    cout << m1 << endl;

    Mat m2;
    m2.create(4, 5, CV_8UC2);

    //点的表示
    Point p;
    p.x = 1;
    p.y = 1;

    //颜色的表示
    Scalar(1, 1, 1); //颜色是BGR 不是RGB  注意对应关系

    //尺寸的表示size
    Size(5, 5); //宽度和高度为5

    //矩形的表示 Rect, 成员变量有x,y,width, height
    Rect r1(0, 0, 100, 60);
    Rect r2(10, 10, 100, 60);
    Rect r3 = r1 | r2; //两个矩形求交集
    Rect r4 = r1 & r2; //两个矩形求并集

    //访问图片中的每个像素，指针的方式
    Mat img = imread("../lena.jpg");
    for (int i = 0; i < img.rows; i++)
    {
        uchar *data = img.ptr<uchar>(i); //获取i行的地址
        for (int j = 0; j < img.cols; j++)
        {
            printf("%d\n", data[j]);
        }
    }

    //直方图均衡化
    Mat dst;
    cvtColor(img, img, COLOR_BGR2GRAY);
    equalizeHist(img, dst);
    imshow("直方图均衡化", dst);

    waitKey(0);

    return 0;
}