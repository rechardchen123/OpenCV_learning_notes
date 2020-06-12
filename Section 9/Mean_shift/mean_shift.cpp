/*
Use the MeanShift to track the human face
1. calculate the human face histogram
2. calculate the back project
3. use the MeanShift
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap("../face1.mp4");

    Mat frame;
    cap >>frame;

    //detect faces in the image
    CascadeClassifier faceCascade;
    String faceCascadePath = "../models/haarcascade_frontalface_default.xml";

    if(!faceCascade.load(faceCascadePath))
        cout<< "--(!) error loading face cascade."<<endl;
    
    Mat frameGray;
    cvtColor(frame, frameGray, COLOR_BGR2GRAY);

    vector<Rect> faces;
    faceCascade.detectMultiScale(frameGray, faces, 1.3, 5);

    int x = faces[0].x;
    int y = faces[0].y;
    int w = faces[0].width;
    int h = faces[0].height;

    Rect currWindow = Rect((long)x, (long)y, (long)w, (long)h);

    Mat roiobject;

    //get the face region from the frame
    frame(currWindow).copyTo(roiobject);
    Mat hsvObject;
    cvtColor(roiobject, hsvObject, COLOR_BGR2HSV);

    //get the mask for calculating histogram of the object and also remove noise
    Mat mask;
    inRange(hsvObject, Scalar(0, 50, 50), Scalar(180, 256, 256), mask);

    //split the image into channels for finding the histogram
    vector<Mat> channels(3);
    split(hsvObject, channels);

    imshow("Mask of ROI", mask);
    waitKey(0);
    imshow("ROI", roiobject);
    waitKey(0);
    destroyAllWindows();

    Mat histObject;

    //initialize parameters for histogram
    int histSize = 180;
    float range[] = {0, 179};
    const float *ranges[] = {range};

    //find the histogram and normalize it to have values between 0 to 255
    calcHist(&channels[0], 1, 0, mask, histObject, 1, &histSize, ranges, true, false);
    normalize(histObject, histObject, 0, 255, NORM_MINMAX);

    //we will process only first 5 frames
    int count = 0;
    Mat hsv, backProjectImage, frameClone;
    while (1)
    {
        //read frame
        cap >>frame;
        if(frame.empty())
            break;
        
        //conver to hsv color space 
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        split(hsv, channels);

        //find the back projected image with the histogram obtained earlier
        calcBackProject(&channels[0], 1, 0, histObject, backProjectImage, ranges);
        imshow("Back Projected Image", backProjectImage);
        waitKey(0);

        //compute the new window using mean shift in the present frame
        int ret = meanShift(backProjectImage, currWindow, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

        //Display the frame with the tracked location of face
        frameClone = frame.clone();

        rectangle(frameClone, Point(currWindow.x, currWindow.y), Point(currWindow.x + currWindow.width, currWindow.y + currWindow.height),
                    Scalar(255, 0,0), 2, LINE_AA);
        imshow("Mean shift object tracking demo", frameClone);
        waitKey(0);

        count += 2;
        if(count == 10)
            break;
    }

    cap.release();
    return 0;
    
}