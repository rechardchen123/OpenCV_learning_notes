#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "skin_smooth1.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::dnn;

string MODEL_PATH = "../face_detector/";
string LANDMARK_PATH = "../models/";

int main(int argc, char** argv){
    const std::string tensorflowConfigFile = MODEL_PATH + "opencv_face_detector.pbtxt";
    const std::string tensorflowWeightFile = MODEL_PATH + "opencv_face_detector_uint8.pb";
    const std::string landmark_xml = LANDMARK_PATH + "facial-landmark-35-adas-0002.xml";
    const std::string landmark_bin = LANDMARK_PATH + "facial-landmark-35-adas-0002.bin";

    //generate mask 
    Mat image = imread("../pink-glasses-hi.png", IMREAD_UNCHANGED);
    Mat bgr;
    cvtColor(image, bgr, COLOR_BGRA2BGR);
    Mat mask = Mat::zeros(image.size(), CV_8UC1);
    int w = image.cols;
    int h = image.rows;
    for(int row = 0; row < h; row++){
        for(int col = 0; col < w; col++){
            Vec4b bgra = image.at<Vec4b>(row, col);
            if(bgra[3]>0){
                mask.at<uchar>(row, col) = 255;
            }
        }
    }
    imshow("mask", mask);

    //load LANDMARK
    Net mkNet = readNetFromModelOptimizer(landmark_xml, landmark_bin);
    mkNet.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
	mkNet.setPreferableTarget(DNN_TARGET_CPU);

    //load network
    setInferenceEngineBackendType(CV_DNN_BACKEND_INFERENCE_ENGINE_NN_BUILDER_API);
    Net net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
    net.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    net.setPreferableTarget(DNN_TARGET_CPU);

    //open the camera
    VideoCapture cap(0);
    w = cap.get(CAP_PROP_FRAME_WIDTH);
    h = cap.get(CAP_PROP_FRAME_HEIGHT);
    vector<Point> mkList;
    FaceMakeUp fm;
    Mat frame;
    while(true){
        bool ret = cap.read(frame);
        if(!ret) break;
        flip(frame, frame, 1);
        cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), Scalar(104,177,123), false, false);
        net.setInput(inputBlob, "data");
        cv::Mat detection = net.forward("detection_out");
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        for(int i = 0; i < detectionMat.rows; i++){
            float confidence = detectionMat.at<float>(i, 2);
            if(confidence > 0.5){
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3)* w);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4)* h);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5)* w);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6)* h);
                Mat roi = frame(Range(y1, y2), Range(x1, x2));
                Mat blob = blobFromImage(roi, 1.0, Size(60, 60), Scalar(), false, false);
                mkNet.setInput(blob);
                Mat landmark_data = mkNet.forward();
                mkList.clear();
                for(int i = 0; i < landmark_data.cols; i +=2){
                    float x = landmark_data.at<float>(0, i) * roi.cols +x1;
                    float y = landmark_data.at<float>(0, i+1)*roi.rows + y1;
                    mkList.push_back(Point(x,y));
                    circle(frame, Point(x, y), 2, Scalar(0, 0, 255), 2, 0);
                }
                if(mkList.size() == 35){
                    fm.smoothSkin(roi);
                    fm.wearMask(frame, bgr, mask, mkList);
                }
                cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2, 8);
            }
        }
        imshow("Face-Detection", frame);
        char c = waitKey(1);
        if (c == 27)
            break;
    }
    waitKey(0);
    cap.release();
    return 0;
}