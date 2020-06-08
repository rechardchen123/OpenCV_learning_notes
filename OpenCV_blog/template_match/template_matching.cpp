#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img, templ, result;
    img = imread("../1.jpg");
    templ = imread("../2.png");

    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create(result_cols, result_rows, CV_32FC1);

    matchTemplate(img, templ, result, TM_SQDIFF_NORMED); //这里我们使用的匹配算法是标准平方差匹配 method=CV_TM_SQDIFF_NORMED，数值越小匹配度越好
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    double minVal = -1;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    cout << "匹配度：" << minVal << endl;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    cout << "匹配度：" << minVal << endl;

    matchLoc = minLoc;
    if (minVal < 0.001)
    {
        rectangle(img, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 2, 8, 0);
    }

    imshow("img", img);

    waitKey(0);
    return 0;
}