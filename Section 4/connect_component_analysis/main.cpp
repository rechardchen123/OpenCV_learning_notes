#include <iostream>
#include <opencv2/opencv.hpp>

using namespace  std;
using namespace cv;
int main() {
    Mat image = imread("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 4\\connect_component_analysis\\1.png", IMREAD_GRAYSCALE);
    imshow("input", image);
    //Threshold Image
    Mat imThresh;
    threshold(image, imThresh, 127, 255, THRESH_BINARY);
    //Find connected components
    Mat imLabels;
    int nComponents = connectedComponents(imThresh, imLabels);
    Mat imLabelsCopy = imLabels.clone();
    //First let's find the min and max values in imLabels
    Point minLoc, maxLoc;
    double minVal, maxVal;
    //The following line finds the min and max pixel values
    // and their locations in an image.
    minMaxLoc(imLabels, &minVal, &maxVal, &minLoc, &maxLoc);
    //Normalise the image so the min value us 0 and max value is 255.
    imLabels = 255 * (imLabels - minVal) / (maxVal - minVal);
    //convert image to 8-bits
    imLabels.convertTo(imLabels, CV_8U);
    //Apply a color map
    Mat imColorMap;
    applyColorMap(imLabels, imColorMap, COLORMAP_JET);
    //Display a color map
    imshow("image", imColorMap);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
