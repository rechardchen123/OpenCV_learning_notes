/*************************************************************************
    > File Name: Hough_lines.cpp
    > Author: Xiang Chen
    > Mail: rechardchen123@outlook.com
    > Created Time: 2020-06-06
	> Descrpitions: The Hough transformation
 ************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = cv::imread("../1.jpg");
	cv::imshow("src Image", srcImage);
	Mat midImage, dstImage;
	cv::Canny(srcImage, midImage, 50, 200, 3);
	cv::cvtColor(midImage, dstImage, COLOR_GRAY2BGR);
	//define a vector to save the detected line
	//vector<Vec2f> lines;
	//through the HoughLines to detect the line set
	//HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);

	//与HoughLines不同的是，HoughLinesP得到lines的是含有直线上点的坐标的，所以下面进行划线时就不再需要自己求出两个点来确定唯一的直线了
	vector<Vec4i> lines;
	HoughLinesP(midImage, lines, 1, CV_PI / 180, 80, 50, 10);
	//The fifth parameter indicates that the threshold is larger, the accuracy is more.

	//draw every line
	/*for (int i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];   //circle's radius
		float theta = lines[i][1]; // line's angle
		Point pt1, pt2;
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho;
		double y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		line(dstImage, pt1, pt2, Scalar(55, 100, 195), 1, LINE_AA);
		cout << "line " << i << ":"
			 << "rho " << rho << "theta " << theta << endl;
		imshow("边缘检测后的图", midImage);
		imshow("最终效果图", dstImage);
	}*/

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);
		imshow("边缘检测后的图", midImage);
		imshow("最终效果图", dstImage);
	}

	waitKey(0);
	return 0;
}
