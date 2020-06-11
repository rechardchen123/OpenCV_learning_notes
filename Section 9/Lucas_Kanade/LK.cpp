/*
Lucas_Kanade method is one of optic flow algorithms. It uses the image pyramid to calculate. 

*/
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

// fill the vector with random colors
void getRandomColors(vector<Scalar> &colors, int numColors)
{
    RNG rng(0);
    for (int i = 0; i < numColors; i++)
    {
        colors.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
    }
}

int main()
{
    string videoFrame = "../cycle.mp4";
    VideoCapture cap(videoFrame);

    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    VideoWriter out("sparse-output.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 20, Size(width, height));

    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 10, 0.03);

    //take first frame and find corners in it
    Mat old_frame;
    cap >> old_frame;

    Mat old_gray;
    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);

    vector<Point2f> old_points;
    vector<uchar> status;
    vector<float> err;
    vector<Point2f> new_points;

    vector<Point2f> good_new;
    vector<Point2f> good_old;

    vector<Scalar> colors;

    Point2f pt1, pt2;

    //Shi Tomasi corner detection to find the feature points.
    goodFeaturesToTrack(old_gray, old_points,
                        100,   //maxCorners
                        0.3,   //quality level
                        7,     //minDistance
                        Mat(), //mask
                        7      //blocksize
    );
    Mat display_frame;

    //create a mask image for drawing the tracks
    Mat mask = Mat::zeros(old_frame.size().height, old_frame.size().width, CV_8UC3);

    int count = 0;

    Mat frame, frame_gray;

    while (1)
    {
        cap >> frame;
        if (frame.empty())
        {
            cout << "over" << endl;
        }
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        count += 1;

        //calculate optical flow
        calcOpticalFlowPyrLK(old_gray,
                             frame_gray,
                             old_points,
                             new_points,
                             status,
                             err,
                             Size(15, 15),
                             2, termcrit);

        for (int i = 0; i < new_points.size(); i++)
        {
            if (status[i] == 1)
            {
                good_new.push_back(new_points[i]);
                good_old.push_back(old_points[i]);
            }
        }

        getRandomColors(colors, new_points.size());

        //draw the tracks
        for (int j = 0; j < new_points.size(); j++)
        {
            pt1 = new_points[j];
            pt2 = old_points[j];
            line(mask, pt1, pt2, colors[j], 2, LINE_AA);
            circle(frame, pt1, 3, colors[j], -1);
        }

        add(frame, mask, display_frame);
        out.write(display_frame);

        if (count % 5 == 0)
        {
            imshow("image", display_frame);
            waitKey(10);
        }
        if (count > 50)
            break;

        // now update the previous frame and previous_points
        old_gray = frame_gray.clone();
        std::copy(new_points.begin(), new_points.end(), old_points.begin());
    }
    cap.release();
    out.release();

    return 0;
}