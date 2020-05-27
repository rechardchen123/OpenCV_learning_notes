#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>

using namespace cv::ml;
using namespace cv;
using namespace std;

void getFileNames(string dirName, vector<string> &imageFnames, string imgExt)
{
    glob(dirName, imageFnames, false);
}

int main()
{
    // Initialize HOG
    HOGDescriptor hog(
        Size(64, 128), //winSize
        Size(16, 16),  //blocksize
        Size(8, 8),    //blockStride,
        Size(8, 8),    //cellSize,
        9,             //nbins,
        0,             //derivAperture,
        -1,            //winSigma,
        HOGDescriptor::L2Hys,
        0.2, //L2HysThresh,
        1,   //gammal correction,
        64,  //nlevels=64
        0);  //signedGradient

    Ptr<SVM> svm = ml::SVM::load("../pedestrian.yml");
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    vector<float> svmDetectorTrained;
    svmDetectorTrained.clear();
    svmDetectorTrained.resize(sv.cols + 1);
    for (int j = 0; j < sv.cols; j++)
    {
        svmDetectorTrained[j] = -sv.at<float>(0, j);
    }
    svmDetectorTrained[sv.cols] = (float)rho;

    // set SVMDetector trained by us in HOG
    hog.setSVMDetector(svmDetectorTrained);

    // OpenCV's HOG based Pedestrian Detector
    HOGDescriptor hogDefault(
        Size(64, 128), //winSize
        Size(16, 16),  //blocksize
        Size(8, 8),    //blockStride,
        Size(8, 8),    //cellSize,
        9,             //nbins,
        0,             //derivAperture,
        -1,            //winSigma,
        HOGDescriptor::L2Hys,
        0.2, //L2HysThresh,
        1,   //gammal correction,
        64,  //nlevels=64
        0);  //signedGradient
             // Set the people detector.
    vector<float> svmDetectorDefault = hog.getDefaultPeopleDetector();
    hogDefault.setSVMDetector(svmDetectorDefault);

    string imageDir = "C:/Users/ucesxc0/Desktop/OpenCV/section8/pedestrians/pedestrians/";
    vector<string> imagePaths;
    getFileNames(imageDir, imagePaths, "jpg");

    float finalHeight = 800.0;
    for (int i = 0; i < imagePaths.size(); i++)
    {
        cout << "processing: " << imagePaths[i] << endl;
        Mat im = imread(imagePaths[i], IMREAD_COLOR);

        float finalWidth = (finalHeight * im.cols) / im.rows;
        resize(im, im, Size(finalWidth, finalHeight));

        vector<Rect> bboxes, bboxes2;
        vector<double> weights, weights2;

        float hitThreshold = 1.0;
        Size winStride = Size(8, 8);
        Size padding = Size(32, 32);
        float scale = 1.05;
        float finalThreshold = 2;
        bool useMeanshiftGrouping = 0;
        hog.detectMultiScale(im, bboxes, weights, hitThreshold, winStride, padding,
                             scale, finalThreshold, useMeanshiftGrouping);

        hogDefault.detectMultiScale(im, bboxes2, weights2, 0, winStride, padding,
                                    scale, finalThreshold, useMeanshiftGrouping);

        if (!bboxes.empty())
        {
            cout << "Trained Detector :: pedestrians detected: " << bboxes.size() << endl;
            vector<Rect>::const_iterator loc = bboxes.begin();
            vector<Rect>::const_iterator end = bboxes.end();
            for (; loc != end; ++loc)
            {
                rectangle(im, *loc, Scalar(0, 255, 0), 2);
            }
        }
        if (!bboxes2.empty())
        {
            cout << "Default Detector :: pedestrians detected: " << bboxes2.size() << endl;
            vector<Rect>::const_iterator loc = bboxes2.begin();
            vector<Rect>::const_iterator end = bboxes2.end();
            for (; loc != end; ++loc)
            {
                rectangle(im, *loc, Scalar(0, 0, 255), 2);
            }
        }

        imshow("pedestrians", im);
        imwrite("results/" + imagePaths[i], im);
        waitKey(0);
    }
    return 0;
}
