
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mean_filter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiang chen  <rechardchen123@outlook.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 09:36:52 by xiang chen        #+#    #+#             */
/*   Updated: 2020/06/03 09:36:52 by xiang chen       ###   ########.fr       */
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
    Mat DstPic;
    blur(srcPic, DstPic, Size(7, 7));
    imshow("均值模糊效果图", DstPic);
    waitKey();
    return 0;
}