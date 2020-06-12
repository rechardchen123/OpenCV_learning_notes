/*
single object trakcing algorithms
1. boosting
2. Multiple Instance Learning (MIL)
3. Kernelized Correlation Filters (KCF)
4. Tracking learning and detection (TLD)
5. MedianFlow
6. GOTURN
7. Mimimum Output Sum of Squared Error (MOSSE)
8. Channel and Spatial Reliability(CSRT)
*/

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

using namespace std;
using namespace cv;

//convert to string
#define SSTR(x) static_cast<std::ostringstream &>((std::ostringstream() << std::dec << x)).str()

int main()
{
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "CSRT", "MOSSE"};

    //create a tracker
    string trackerType = trackerTypes[6];

    Ptr<Tracker> tracker;
    if (trackerType == "BOOSTING")
        tracker = TrackerBoosting::create();
    else if (trackerType == "MIL")
        tracker = TrackerMIL::create();
    else if (trackerType == "KCF")
        tracker = TrackerKCF::create();
    else if (trackerType == "TLD")
        tracker = TrackerTLD::create();
    else if (trackerType == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    else if (trackerType == "GOTURN")
        tracker = TrackerGOTURN::create();
    else if (trackerType == "CSRT")
        tracker = TrackerCSRT::create();
    else if (trackerType == "MOSSE")
        tracker = TrackerCSRT::create();
    else
    {
        cout << "Invalid tracker specified." << endl;
        cout << "Available Tracker are :" << endl;
        for (int i = 0; i < sizeof(trackerTypes) / sizeof(trackerTypes[0]); i++)
            cout << i << " : " << trackerTypes[i] << endl;
        return -1;
    }

    //read the video
    VideoCapture video("../hockey.mp4");

    //exit if video is not opened
    if (!video.isOpened())
    {
        cout << "could not read video file" << endl;
        return -1;
    }

    //read first frame
    Mat frame;
    bool ok = video.read(frame);

    //define initial bounding box
    Rect2d bbox(204, 131, 97, 222);

    cout << "Initial bounding box: " << bbox << endl;

    //Display bounding box
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    imshow("tracking", frame);

    tracker->init(frame, bbox);

    while (video.read(frame))
    {
        //start timer
        double timer = (double)getTickCount();
        //update the tracking result
        bool ok = tracker->update(frame, bbox);

        //calculate frames per second
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            putText(frame, "tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }
        // display tracker type on frame
        putText(frame, trackerType + " tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        //display FPS on frame
        putText(frame, "FPS: " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        imshow("Tracking", frame);

        int k = waitKey(1);
        if (k == 27)
        {
            break;
        }
    }
}