/*
Use the caffe model to classify the images

*/

#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

std::vector<std::string> classes;

int main()
{
    string protoFile = "/home/richardchen123/Documents/data/models/bvlc_googlenet.prototxt";
    string weightFile = "/home/richardchen123/Documents/data/models/bvlc_googlenet.caffemodel";

    Mat frame = imread("../data/images/panda.jpg");

    string classFile = "/home/richardchen123/Documents/data/models/classification_classes_ILSVRC2012.txt";
    ifstream ifs(classFile.c_str());
    string line;
    while (getline(ifs, line))
    {
        classes.push_back(line);
    }

    float scale = 1.0;
    int inHeight = 224;
    int inWidth = 224;
    bool swapRB = false;
    Scalar mean = Scalar(104, 117, 123);

    //read and initialize network
    Net net = readNetFromCaffe(protoFile, weightFile);

    //process frames
    Mat blob;

    //create a 4D blob from a frame
    blobFromImage(frame, blob, scale, Size(inWidth, inHeight), mean, swapRB, false);

    //set input blob
    net.setInput(blob);

    //make forward pass
    Mat prob = net.forward();

    //get a class with a highest score
    Point classIdPoint;
    double confidence;
    minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
    int classId = classIdPoint.x;

    //print predicted class
    string label = format("Predicted class: %s, confidence: %.3f", (classes[classId].c_str()), confidence);
    cout << label << endl;
    putText(frame, label, Point(30, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2, LINE_AA);

    imshow("Classification Output", frame);
    waitKey(0);
    destroyAllWindows();
    return 0;
}