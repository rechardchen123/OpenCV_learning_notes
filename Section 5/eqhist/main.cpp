#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
void equalHSV(){
    Mat img = imread("../dark-flowers.jpg");
    imshow("input", img);
    // Convert to HSV color space
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    vector<Mat> hsvChannels;
    split(hsv, hsvChannels);
    vector<Mat> rgb;
    split(img, rgb);
    for(int i = 0; i < rgb.size(); i++){
        equalizeHist(rgb[i], rgb[i]);
    }
    equalizeHist(hsvChannels[2], hsvChannels[2]);
    merge(hsvChannels, hsv);
    Mat dst_v;
    cvtColor(hsv, dst_v, COLOR_HSV2BGR);
    imshow("HSV effective", dst_v);
    Mat dst;
    merge(rgb, dst);
    imshow("RGB equalization", dst);
    waitKey(0);
}

void CLAHE_function(){
    Mat img = imread("../night-sky.jpg");
    imshow("input", img);
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    vector<Mat> hsvChannels;
    split(hsv, hsvChannels);

    equalizeHist(hsvChannels[2], hsvChannels[2]);
    merge(hsvChannels, hsv);
    Mat dst_v;
    cvtColor(hsv, dst_v, COLOR_HSV2BGR);
    //CLAHE(Contrast Limited Adaptive Histogram Equalization)
    auto clahe = createCLAHE(2.0, Size(8,8));
    vector<Mat> chsv;
    split(hsv, chsv);
    clahe->apply(chsv[2], chsv[2]);
    Mat dst;
    merge(chsv, dst);
    cvtColor(dst, dst, COLOR_HSV2BGR);
    imshow("CLAHE", dst);
    waitKey(0);
}

void interpolation(uchar* lut, float* fullRange, float* Curve, float* originalValue){
    for(int i = 0; i < 256; i++){
        int j = 0;
        float a = fullRange[i];
        while(a > originalValue[j]){
            j++;
        }
        if(a == originalValue[j]){
            lut[i] = Curve[j];
            continue;
        }
        float slope = ((float)(Curve[j] - Curve[j-1]))/(originalValue[j] - originalValue[j-1]);
        float constant = Curve[j] - slope* originalValue[j];
        lut[i] = slope * fullRange[i] + constant;
    }
}

int main() {
    //equalHSV();
    //CLAHE_function();
    Mat Image = imread("../girl.jpg");
    //pivot points for x-coordinates
    float originalValue[] = {0,50,100,150,200,255};
    //change points on Y-axis for each channel
    float rCurve[] = {0,80,150,190,220,255};
    float bCurve[] = {0,20,40,75,150,255};

    //split the channels
    vector<Mat> channels(3);
    split(Image, channels);

    //create a LookUp Table
    float fullRange[256];
    for(int i = 0; i < 256; i++){
        fullRange[i] = (float) i;
    }
    Mat lookUpTable(1, 256, CV_8U);
    uchar* lut = lookUpTable.ptr();
    //apply inter[olation and create look up table
    interpolation(lut, fullRange, rCurve, originalValue);

    // Apply mapping and check for underflow/overflow in Red Channel
    LUT(channels[2], lookUpTable, channels[2]);
    min(channels[2], 255, channels[2]);
    max(channels[2], 0, channels[2]);

    // Apply interpolation and create look up table
    interpolation(lut, fullRange, bCurve, originalValue);

    // Apply mapping and check for underflow/overflow in Blue Channel
    LUT(channels[0], lookUpTable, channels[0]);
    min(channels[0], 255, channels[0]);
    max(channels[0], 0, channels[0]);

    Mat output;
    // Merge the channels
    merge(channels, output);

    // Display the two images in one window
    Mat combined;
    cv::hconcat(Image, output, combined);

    imshow("Warming Filter Output", combined);
    waitKey(0);
    return 0;
}
