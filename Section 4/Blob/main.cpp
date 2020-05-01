#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main() {
    string imagePath = "C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding"
                       "\\OpenCV_learning_notes\\Section 4\\Blob\\blob_detection.jpg";
    Mat img = imread(imagePath, IMREAD_GRAYSCALE);
    imshow("input", img);
    //Set up detector with default parameters
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();

    std::vector<KeyPoint> keypoints;
    detector->detect(img, keypoints);

    int x,y;
    int radius;
    double diameter;
    cvtColor(img, img, COLOR_GRAY2BGR);
    for(int i = 0; i< keypoints.size(); i++){
        KeyPoint k = keypoints[i];
        Point keyPt;
        x = (int)keyPt.x;
        y = (int)keyPt.y;
        //mark center in Black
        circle(img, Point(x,y), 5, Scalar(255,0,0),-1);
        //get radius of coin
        diameter = k.size;
        radius = (int)diameter/2.0;
        //mark blob in GREEN
        circle(img, Point(x,y), radius, Scalar(0,255,0),2);
    }
    imshow("Blob detect",img);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
