#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*define the global variables */
Mat img;
bool selectedObj = false;
int trackObj = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 0, vmax = 256, smin = 30;

int main()
{
    VideoCapture cap("../balltest.mp4");
    //VideoCapture cap(0);
    Rect trackWindow;
    int hsize = 16;
    float hranges[] = {0, 180};
    const float *phranges = hranges;

    if (!cap.isOpened())
        return -1;

    namedWindow("Histogram", WINDOW_AUTOSIZE);
    namedWindow("result", WINDOW_AUTOSIZE);

    Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool paused = false;
    cap.read(frame);
    Rect selection = selectROI("result", frame, true, false);
    vector<Point> tracking_path;

    while (true)
    {
        bool ret = cap.read(frame);
        if (!ret)
            break;
        frame.copyTo(img);
        cvtColor(img, hsv, COLOR_BGR2HSV);

        int _vmin = vmin, _vmax = vmax;
        inRange(hsv, Scalar(26, 43, 46), Scalar(34, 255, 255), mask);
        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);

        if (trackObj <= 0)
        {
            // Object has been selected by user, set up CAMShift search properties once
            Mat roi(hue, selection), maskroi(mask, selection);
            calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
            normalize(hist, hist, 0, 255, NORM_MINMAX);

            trackWindow = selection;
            trackObj = 1; // Don't set up again, unless user selects new ROI

            histimg = Scalar::all(0);
            int binW = histimg.cols / hsize;
            Mat buf(1, hsize, CV_8UC3);
            for (int i = 0; i < hsize; i++)
                buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i * 180. / hsize), 255, 255);
                cvtColor(buf, buf, COLOR_HSV2BGR);

            for (int i = 0; i < hsize; i++)
            {
                int val = saturate_cast<int>(hist.at<float>(i) * histimg.rows / 255);
                rectangle(histimg, Point(i * binW, histimg.rows),
                          Point((i + 1) * binW, histimg.rows - val),
                          Scalar(buf.at<Vec3b>(i)), -1, 8);
            }
        }

        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        RotatedRect trackBox = CamShift(backproj, trackWindow,
                                        TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
        if (trackBox.center.x > 0 && trackBox.center.y > 0)
            tracking_path.push_back(trackBox.center);
        ellipse(img, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
        for (int i = 1; i < tracking_path.size(); i++)
        {
            line(img, tracking_path[i - 1], tracking_path[i], Scalar(255, 0, 0), 2, 8, 0);
        }

        imshow("result", img);
        imshow("Histogram", histimg);
        char c = (char)waitKey(50);
        if (c == 27)
            break;
    }
    return 0;
}