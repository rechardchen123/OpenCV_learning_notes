/*
Video stable:
1. input the video frame
2. find the feature points
3. calculate adjacent features using optical flow 
4. calculate the feature point's motion character
5. smooth the trajectory
6. generate new image series
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

using namespace std;
using namespace cv;

//In frmaes, the larger the more stable the video, but less reactive to sudden planning
const int SMOOTHING_RADIUS = 50;

struct TransformParam{
    TransformParam(){}
    TransformParam(double _dx, double _dy, double _da){
        dx = _dx;
        dy = _dy;
        da = _da;
    }
    double dx;
    double dy;
    double da; //angle

    void getTransform(Mat & T){
        //reconstruct transformation matrix accordingly to new values
        T.at<double>(0,0) = cos(da);
        T.at<double>(0,1) = -sin(da);
        T.at<double>(1,0) = sin(da);
        T.at<double>(1,1) = cos(da);

        T.at<double>(0,2) = dx;
        T.at<double>(1,2) = dy;
    }
};

struct Trajectory{
    Trajectory(){}
    Trajectory(double _x, double _y, double _a){
        x = _x;
        y = _y;
        a = _a;
    }
    double x;
    double y;
    double a;
};

auto cumsum(vector<TransformParam> &transforms){
    vector<Trajectory> trajectory; //trajectory all frames
    //accumulated frame to frame transform
    double a = 0;
    double x = 0;
    double y = 0;

    for(size_t i = 0; i < transforms.size(); i++){
        x += transforms[i].dx;
        y += transforms[i].dy;
        a += transforms[i].da;

        trajectory.push_back(Trajectory(x,y,a));
    }
    return trajectory;
}

auto smooth(vector<Trajectory> &trajectory, int radius){
    vector<Trajectory> smoothed_trajectory;
    for(size_t i = 0; i < trajectory.size(); i++){
        double sum_x = 0;
        double sum_y = 0;
        double sum_a = 0;
        int count = 0;
        
        for(int j = -radius; j<radius;j++){
            if(i+j >=0 && i+j<trajectory.size()){
                sum_x += trajectory[i+j].x;
                sum_y += trajectory[i+j].y;
                sum_a += trajectory[i+j].a;

                count ++;
            }
        }
        double avg_a = sum_a / count;
        double avg_x = sum_x / count;
        double avg_y = sum_y / count;

        smoothed_trajectory.push_back(Trajectory(avg_x, avg_y, avg_a));
    }
    return smoothed_trajectory;
}

void fixBorder(Mat &frame_stabilized){
    Mat T = getRotationMatrix2D(Point2f(frame_stabilized.cols/2, frame_stabilized.rows/2),0,1.04);
    warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size());
}

int main(){
    //read the input video
    VideoCapture cap("../video.mp4");

    //get frame count
    int n_frames = int(cap.get(CAP_PROP_FRAME_COUNT));

    //get the width and height of video stream
    int w = int(cap.get(CAP_PROP_FRAME_WIDTH));
    int h = int(cap.get(CAP_PROP_FRAME_HEIGHT));

    //get frames per second (fps)
    double fps = cap.get(CAP_PROP_FPS);

    //set up output video 
    VideoWriter out("../video_out.avi", VideoWriter::fourcc('M','J','P','G'), fps, Size(2*w, h));

    //Define variable for storing frames
    Mat curr, curr_gray;
    Mat prev, prev_gray;

    //read the first frame
    cap >> prev;

    //convert frame to grayscale
    cvtColor(prev, prev_gray, COLOR_BGR2GRAY);

    //pre-define transformation-store array
    vector<TransformParam> transforms;

    Mat last_T;

    for(int i = 0; i<n_frames -1; i++){
        //vector from previos and current feature points
        vector<Point2f> prev_pts, curr_pts;
        
        //Detect features in previous frame
        goodFeaturesToTrack(prev_gray, prev_pts,200, 0.01, 30);

        //read next frame
        bool success = cap.read(curr);
        if(!success) break;

        //convert to grayscale
        cvtColor(curr, curr_gray, COLOR_BGR2GRAY);

        //calculate optical flow 
        vector<uchar> status;
        vector<float> err;
        calcOpticalFlowPyrLK(prev_gray, curr_gray, prev_pts, curr_pts, status, err);

        //filter only valid points
        auto prev_it = prev_pts.begin();
        auto curr_it = curr_pts.begin();
        for(size_t k = 0; k < status.size(); k++){
            if(status[k]){
                prev_it++;
                curr_it++;
            }
            else{
                prev_it = prev_pts.erase(prev_it);
                curr_it = curr_pts.erase(curr_it);
            }
        }

        //find transformation matrix 
        Mat T = estimateAffinePartial2D(prev_pts, curr_pts);
        
        //in rare cases no transform is found. 
        //we will just use the last known good transform.
        if(T.data == NULL) last_T.copyTo(T);
        T.copyTo(last_T);

        //extract traslation
        double dx = T.at<double>(0,2);
        double dy = T.at<double>(1,2);

        //extract roation angle
        double da = atan2(T.at<double>(1,0), T.at<double>(0,0));

        //store transformation
        transforms.push_back(TransformParam(dx,dy,da));

        //move to next frame
        curr_gray.copyTo(prev_gray);

        cout<<"frame: "<<i << "/"<<n_frames << "-  Tracked points: "<<prev_pts.size()<<endl;
    }

    //compute trajectory using cummulative sum of transformations
    vector<Trajectory> trajectory = cumsum(transforms); 

    //smooth trajectory using moving average filter
    vector<Trajectory> smoothed_trajectory = smooth(trajectory, SMOOTHING_RADIUS);

    vector<TransformParam> transforms_smooth;

    for(size_t i = 0; i < transforms.size(); i++){
        //calculate difference in smoothed_trajectory and trajectory
        double diff_x = smoothed_trajectory[i].x - trajectory[i].x;
        double diff_y = smoothed_trajectory[i].y - trajectory[i].y;
        double diff_a = smoothed_trajectory[i].a - trajectory[i].a;

        //calculate newer transformation array
        double dx = transforms[i].dx + diff_x;
        double dy = transforms[i].dy + diff_y;
        double da = transforms[i].da + diff_a;

        transforms_smooth.push_back(TransformParam(dx, dy, da));
    }

    cap.set(CAP_PROP_POS_FRAMES, 0);
    Mat T(2,3,CV_64F);
    Mat frame, frame_stabilized, frame_out;

    for(int i = 0; i < n_frames -1; i++){
        bool success = cap.read(frame);
        if(!success) break;

        //extract transform translation and rotation angle
        transforms_smooth[i].getTransform(T);

        //apply affine wrapping to the given frame
        warpAffine(frame, frame_stabilized, T, frame.size());

        //sclae image to remove black border artifact
        fixBorder(frame_stabilized);

        //now draw the original and stablised side by side for coolness
        hconcat(frame, frame_stabilized, frame_out);

        //If the image is too big, resize it
        if(frame_out.cols > 200){
            resize(frame_out, frame_out, Size(frame_out.cols/2, frame_out.rows/2));
        }

        imshow("before and after", frame_out);
        out.write(frame_out);
        waitKey(10);
    }
    cap.release();
    out.release();
    return 0;
}