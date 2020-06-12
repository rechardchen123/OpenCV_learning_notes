#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("../face1.mp4");

    Mat frame;
    cap >> frame;

    // Detect faces in the image
    CascadeClassifier faceCascade;
    String faceCascadePath = "../models/haarcascade_frontalface_default.xml";

    if (!faceCascade.load(faceCascadePath))
    {
        printf("--(!)Error loading face cascade\n");
    };
    Mat frameGray;
    cvtColor(frame, frameGray, COLOR_BGR2GRAY);

    vector<Rect> faces;
    faceCascade.detectMultiScale(frameGray, faces, 1.3, 5);

    int x = faces[0].x;
    int y = faces[0].y;
    int w = faces[0].width;
    int h = faces[0].height;

    Rect currWindow = Rect((long)x, (long)y, (long)w, (long)h);

    Mat roiObject;

    // get the face region from the frame
    frame(currWindow).copyTo(roiObject);
    Mat hsvObject;
    cvtColor(roiObject, hsvObject, COLOR_BGR2HSV);

    // Get the mask for calculating histogram of the object and
    // also remove noise
    Mat mask;
    inRange(hsvObject, Scalar(0, 50, 50), Scalar(180, 256, 256), mask);

    // Split the image into channels for finding the histogram
    vector<Mat> channels(3);
    split(hsvObject, channels);

    imshow("Mask of ROI", mask);
    waitKey(0);
    imshow("ROI", roiObject);
    waitKey(0);
    destroyAllWindows();

    Mat histObject;

    // Initialize parameters for histogram
    int histSize = 180;
    float range[] = {0, 179};
    const float *ranges[] = {range};

    // Find the histogram and normalize it to have values
    // between 0 to 255
    calcHist(&channels[0], 1, 0, mask, histObject, 1, &histSize, ranges, true, false);
    normalize(histObject, histObject, 0, 255, NORM_MINMAX);

    // We will process only first 5 frames
    int count = 0;
    Mat hsv, backProjectImage, frameClone;
    while (1)
    {
        // Read frame
        cap >> frame;
        if (frame.empty())
            break;

        // Convert to hsv color space
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        split(hsv, channels);

        // find the back projected image with the histogram obtained earlier
        calcBackProject(&channels[0], 1, 0, histObject, backProjectImage, ranges);
        imshow("Back Projected Image", backProjectImage);
        waitKey(0);
        // Compute the new window using mean shift in the present frame
        RotatedRect rotatedWindow = CamShift(backProjectImage, currWindow, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
        // Display the frame with the tracked location of face
        // Get the rotatedWindow vertices
        Point2f rotatedWindowVertices[4];
        rotatedWindow.points(rotatedWindowVertices);

        // Display the frame with the tracked location of face
        frameClone = frame.clone();

        rectangle(frameClone, Point(currWindow.x, currWindow.y), Point(currWindow.x + currWindow.width, currWindow.y + currWindow.height), Scalar(255, 0, 0), 2, LINE_AA);
        // Display the rotated rectangle with the orientation information
        for (int i = 0; i < 4; i++)
            line(frameClone, rotatedWindowVertices[i], rotatedWindowVertices[(i + 1) % 4], Scalar(0, 255, 0), 2, LINE_AA);
        imshow("Mean Shift Object Tracking Demo", frameClone);
        waitKey(0);

        count += 2;
        if (count == 10)
            break;
    }

}
