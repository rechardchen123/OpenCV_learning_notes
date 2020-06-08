/*
图像修复技术的原理是: 
利用那些已经被破坏的区域的边缘， 即边缘的颜色和结构，根据这些图像留下的信息去推断被破坏的信息区的信息内容，
然后对破坏区进行填补 ，以达到图像修补的目的。
OpenCV中就是利用inpaint()这个函数来实现修复功能的
void inpaint( InputArray src, InputArray inpaintMask, OutputArray dst, double inpaintRadius, int flags );
第一个参数src，输入的单通道或三通道图像；
第二个参数inpaintMask，图像的掩码，单通道图像，大小跟原图像一致，inpaintMask图像上除了需要修复的部分之外其他部分的像素值全部为0；
第三个参数dst，输出的经过修复的图像；
第四个参数inpaintRadius，修复算法取的邻域半径，用于计算当前像素点的差值；
第五个参数flags，修复算法，有两种：INPAINT_NS 和I NPAINT_TELEA；
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Point ptL, ptR; //鼠标画出矩形框的起点和终点
Mat imageSource, imageSourceCopy;
Mat ROI; //原图需要修复区域的ROI

//鼠标回调函数
void local_repairing(int event, int x, int y, int flag, void *ustg)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        ptL = Point(x, y);
        ptR = Point(x, y);
    }

    if (flag == EVENT_FLAG_LBUTTON)
    {
        ptR = Point(x, y);
        imageSourceCopy = imageSource.clone();
        rectangle(imageSourceCopy, ptL, ptR, Scalar(255, 0, 0));
        imshow("原图", imageSourceCopy);
    }

    if (event == EVENT_LBUTTONUP)
    {
        if (ptL != ptR)
        {
            ROI = imageSource(Rect(ptL, ptR));
            imshow("ROI", ROI);
            waitKey();
        }
    }

    //单击鼠标右键开始图像修复
    if (event == EVENT_RBUTTONDOWN)
    {
        imageSourceCopy = ROI.clone();
        Mat imageGray;
        cvtColor(ROI, imageGray, COLOR_RGB2GRAY); //转换为灰度图
        Mat imageMask = Mat(ROI.size(), CV_8UC1, Scalar::all(0));

        //通过阈值处理生成Mask
        threshold(imageGray, imageMask, 235, 255, THRESH_BINARY);
        Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        dilate(imageMask, imageMask, Kernel);           //对Mask膨胀处理
        inpaint(ROI, imageMask, ROI, 9, INPAINT_TELEA); //图像修复
        imshow("Mask", imageMask);
        imshow("修复后", imageSource);
    }
    
}

void global_repairing()
{
    Mat imageSrc = imread("../1.png");
    if (!imageSrc.data)
    {
        cout << "cannot find the image." << endl;
    }
    imshow("origin img", imageSrc);
    Mat imageGray;
    cvtColor(imageSrc, imageGray, COLOR_BGR2GRAY, 0);
    Mat imageMask = Mat(imageSrc.size(), CV_8UC1, Scalar::all(0));

    //通过阈值处理生成Mask
    threshold(imageGray, imageMask, 240, 255, THRESH_BINARY);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    //对Mask膨胀处理，增加Mask面积
    dilate(imageMask, imageMask, kernel);

    //图像修复
    inpaint(imageSrc, imageMask, imageSrc, 5, INPAINT_TELEA);
    imshow("Mask", imageMask);
    imshow("修复后", imageSource);
    waitKey(0);
}

int main()
{

    //global repairing effect
    global_repairing();

    //local repairing effect
    imageSource = imread("../1.png");
    imshow("origin image", imageSource);
    setMouseCallback("origin image", local_repairing);

    waitKey(0);
    return 0;
}