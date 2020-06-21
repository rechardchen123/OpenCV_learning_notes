#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    Mat image, mask;
    Rect r1(100, 100, 250, 300);
    Mat img1, img2, img3, img4;
    image = imread("../1.png");
    mask = Mat::zeros(image.size(), CV_8UC1);  //第一步建立与原图一样大小的mask图像，并将所有像素初始化为0，因此全图成了一张全黑色图
    mask(r1).setTo(255); //第二步将mask图中的r1区域的所有像素值设置为255,也就是整个r1区域变成了白色
    img1 = image(r1);
    image.copyTo(img2, mask);

    image.copyTo(img3);
	img3.setTo(0, mask);

    imshow("Image sequence", image);
	imshow("img1", img1);
	imshow("img2", img2);
	imshow("img3", img3);
	imshow("mask", mask);

	waitKey();
	return 0;

}
