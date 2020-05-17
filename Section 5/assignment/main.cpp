// assignment: auto focus
// 1. use the Laplace absolute value variance
// 2. use the improved Laplace energy
// Author: Richard_chen
// Date: 18/05/2020

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void method_one() {
  // open the video file
  VideoCapture capture;
  capture.open("../focus-test.mp4");
  if (!capture.isOpened()) {
    cout << "could not load video..." << endl;
  }

  int h = capture.get(CAP_PROP_FRAME_HEIGHT);  //逐帧获取视频的高度
  int w = capture.get(CAP_PROP_FRAME_WIDTH);   //逐帧获取视频的宽度

  Mat frame, gray;
  float dev = 0;
  float max = -1;
  Mat temp = Mat::zeros(Size(w, h), CV_32FC1);
  Mat mean, stddev;

  while (true) {
    bool ret = capture.read(frame);

    if (!ret) {
      break;
    }
    imshow("frame", frame);
    cvtColor(frame, gray, COLOR_BGR2GRAY);  //将读入的视频帧转换成灰度图像

    for (int row = 1; row < h - 1; row++) {
      for (int col = 1; col < w - 1; col++) {
        int dx = gray.at<uchar>(row, col) * 2 - gray.at<uchar>(row, col + 1) -
                 gray.at<uchar>(row, col - 1);
        int dy = gray.at<uchar>(row, col) * 2 - gray.at<uchar>(row + 1, col) -
                 gray.at<uchar>(row - 1, col);
        temp.at<float>(row, col) = abs(dx + dy);
      }
    }

    meanStdDev(temp, mean, stddev);
    dev = stddev.at<double>(0, 0);
    cout << "stddev: " << dev;

    if (max < dev) {
      max = dev;
      imwrite("../sharpen.png", frame);
    }

    char c = waitKey(1);

    if (c == 27) {
      break;
    }
  }

  Mat result = imread("../sharpen.png");
  imshow("The best sharpness is", result);
  waitKey(0);
  destroyAllWindows();
}

void method_two() {
  VideoCapture capture;
  capture.open("../focus-test.mp4");
  if (!capture.isOpened()) {
    printf("could not load video...\n");
    return;
  }

  int h = capture.get(CAP_PROP_FRAME_HEIGHT);
  int w = capture.get(CAP_PROP_FRAME_WIDTH);
  Mat frame, gray;
  float sum = 0;
  float max = -1;

  while (true) {
    bool ret = capture.read(frame);

    if (!ret) {
      break;
    }

    imshow("frame", frame);
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    for (int row = 1; row < h - 1; row++) {
      for (int col = 1; col < w - 1; col++) {
        int dx = gray.at<uchar>(row, col) * 2 - gray.at<uchar>(row, col + 1) -
                 gray.at<uchar>(row, col - 1);
        int dy = gray.at<uchar>(row, col) * 2 - gray.at<uchar>(row + 1, col) -
                 gray.at<uchar>(row - 1, col);
        sum += (abs(dx) + abs(dy));
      }
    }

    if (max < sum) {
      max = sum;
      imwrite("../sharpen.png", frame);
    }

    sum = 0;
    char c = waitKey(1);

    if (c == 27) {
      break;
    }
  }

  Mat result = imread("../sharpen.png");
  imshow("The best sharpness", result);
  waitKey(0);
  destroyAllWindows;
}

int main() {
  method_one();
  // method_two();
  return 0;
}