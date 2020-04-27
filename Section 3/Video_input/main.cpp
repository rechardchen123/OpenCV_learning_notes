#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void key_reponse(){
    //open webcam
    VideoCapture cap("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\Video_input\\vtest.avi");
    Mat frame;
    int k = 0;

    //detect if it is working properly
    if(!cap.isOpened()){
        cout<<"Unable to open the file..."<<endl;
        return ;
    }
    else{
        while (true){
            //read
            cap.read(frame);
            if(k==27)
                break;
            if(k == 101 || k ==69)
                putText(frame, "E is pressed, press Z or ESC", Point(100,180), FONT_HERSHEY_SIMPLEX,1 ,Scalar(0,25,0),3);
            if(k==90||k==122)
                putText(frame, "Z is pressed", Point(100,180), FONT_HERSHEY_SIMPLEX, 1 ,Scalar(0,255,0),3);
            imshow("Image", frame);
            k = waitKey(10000) & 0xFF;
        }
    }
    cap.release();
    destroyAllWindows();
}


void video_writer(){
    //VideoCapture cap("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\Video_input\\vtest.avi");
    VideoCapture cap(0); //read the local camera device
    //check open
    if(!cap.isOpened()){
        cout<<"Error opening video stream or file."<<endl;
        return;
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter outavi("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\Video_input\\Output.avi",
                       cv::VideoWriter::fourcc('M','J','P','G'), 10, Size(frame_width, frame_height));
    VideoWriter outmp4("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\Video_input\\output.mp4",
                       cv::VideoWriter::fourcc('X','V','I','D'), 10, Size(frame_width,frame_height));
    while(true){
        Mat frame;
        bool ret = cap.read(frame);
        if(!ret){
            break;
        }
        //write the frame
        outavi.write(frame);
        outmp4.write(frame);
        imshow("Frame",frame);
        //ESC
        char c = waitKey(1);
        if(c == 27)
            break;
    }
    //when everything has done, release the video capture object
    cap.release();
    outavi.release();
    outmp4.release();
    //close all window
    destroyAllWindows();
    return;
}

int main() {
    //video_writer();
    key_reponse();
    /*VideoCapture capture;
    capture.open("C:\\Users\\ucesxc0\\Documents\\Repository-My programming and coding\\OpenCV_learning_notes\\Section 3\\Video_input\\vtest.avi");
    if(!capture.isOpened()){
        cout<< "could not opne video file..."<<endl;
        return -1;
    }
    int w = capture.get(CAP_PROP_FRAME_WIDTH);
    int h = capture.get(CAP_PROP_FRAME_HEIGHT);
    int fps = capture.get(CAP_PROP_FPS);
    cout<< "frame width:"<<w<<endl;
    cout<<"frame hight:"<<h<<endl;
    cout<<"frame fps:"<<fps<<endl;
    namedWindow("frame", WINDOW_AUTOSIZE);
    Mat frame;
    while(true){
        //capture >> frame; //read the frame
        bool ret = capture.read(frame);
        if(!ret)
            break;
        imshow("frame", frame);
        char c = waitKey(1);
        if(c==27)   //Esc to quit
            break;
    }
    waitKey(0);
    destroyAllWindows();
    capture.release();
    return 0;*/
}

