/*
SSD object detection
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0 / 127.5;
const float confidenceThreshold = 0.7;
const cv::Scalar meanVal(127.5, 127.5, 127.5);

const std::string configFile = "/home/richardchen123/Documents/data/models/ssd_mobilenet_v2_coco_2018_03_29.pbtxt";
const std::string modelFile = "/home/richardchen123/Documents/data/models/ssd_mobilenet_v2_coco_2018_03_29/frozen_inference_graph.pb";
std::vector<std::string> classes;

const std::string classFile = "/home/richardchen123/Documents/data/models/coco_classes_labels.txt";

cv::Mat detect_objects(cv::dnn::Net net, cv::Mat frame)
{
    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);
    net.setInput(inputBlob);
    Mat detection = net.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    return detectionMat;
}

void display_text(Mat &img, string text, int x, int y)
{
    //get text size
    int baseLine;
    Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, 0.7, 1, &baseLine);

    //use text size to create a black rectangle
    rectangle(img, Point(x, y - textSize.height - baseLine), Point(x + textSize.width, y + baseLine), Scalar(0, 0, 0), -1);

    //display text inside the rectangle
    putText(img, text, Point(x, y - 5), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 1, LINE_AA);
}

void display_objects(Mat &img, Mat objects, float threshold = 0.25)
{
    for (int i = 0; i < objects.rows; i++)
    {
        int classId = objects.at<float>(i, 1);
        float score = objects.at<float>(i, 2);

        //recover original cordinates from normalized coordinates
        int x = static_cast<int>(objects.at<float>(i, 3) * img.cols);
        int y = static_cast<int>(objects.at<float>(i, 4) * img.rows);
        int w = static_cast<int>(objects.at<float>(i, 5) * img.cols - x);
        int h = static_cast<int>(objects.at<float>(i, 6) * img.rows - y);

        //check if the detection is of good quality
        if (score > threshold)
        {
            display_text(img, classes[classId].c_str(), x, y);
            rectangle(img, Point(x, y), Point(x + w, y + h), Scalar(255, 255, 255), 2);
        }
    }
}

int main()
{
    Net net = readNetFromTensorflow(modelFile, configFile);
    ifstream ifs(classFile.c_str());
    string line;
    while (getline(ifs, line))
    {
        classes.push_back(line);
    }

    Mat img, objects;

    img = imread("../data/images/street.jpg");
    objects = detect_objects(net, img);
    imshow("Image", img);
    waitKey(0);
    img = imread("../data/images/baseball.jpg");
    objects = detect_objects(net, img);
    display_objects(img, objects);
    imshow("Image", img);
    waitKey(0);
    destroyAllWindows();
    return 0;
}