/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   image_resize.cpp                                                         */
/*                                                                            */
/*   By: xiang chen  <rechardchen123@outlook.com                              */
/*                                                                            */
/*   Created: 2020/06/04 21:10:56 by xiang chen                               */
/*   Updated: 2020/06/04 21:10:56 by xiang chen                               */
/*   图像缩放一般采用resize函数                                                */
/*   缩小图像一般使用CV_INTER_AREA来插值                                       */
/*   放大图像一般采用CV_INTER_LINEAR                                           */
/* ************************************************************************** */

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

//图像的缩小和放大
int main()
{
    Mat img = imread("../lena.jpg");
    imshow("原始图", img);

    Mat dst = Mat::zeros(512, 512, CV_8UC3);
    resize(img, dst, dst.size());
    imshow("尺寸调整之后", dst);
    waitKey();

    //第二种，填入你要缩小或者放大的比率。
    Mat dst1;
    resize(img, dst1, Size(), 0.5, 0.5); //长宽变为原来的0.5
    imshow("尺寸调整后1", dst1);
    waitKey(0);

    //图像金字塔(image pyramid)
    /* ******************************************************************/
    /* 常见的两种图像金字塔                                              */
    /* 1.高斯金字塔:用于下采样                                           */
    /* 2.拉普拉斯金字塔:用于重建图像                                     */
    /* *****************************************************************/

    //下采样：图片的缩小，使用pyrDown函数
    //下采样步骤：1. 对图像进行高斯内核卷积  2. 将所有偶数行和列去除

    //上采样: 图片的放大，使用pyrUp函数
    //上采样步骤：1. 将图像再每个方向放大原来的两倍，新增的行和列用0填充；
    //           2. 使用先前同样的内核与放大的图像卷积，获取新增图像的近似值

    Mat dst2, dst3;
    pyrUp(img, dst2, Size(img.cols * 2, img.rows * 2));       //放大一倍
    pyrDown(img, dst3, Size(img.cols * 0.5, img.rows * 0.5)); //缩小为原来的一半
    imshow("尺寸放大之后", dst2);
    imshow("尺寸缩小之后", dst3);
    waitKey(0);
    return 0;
}