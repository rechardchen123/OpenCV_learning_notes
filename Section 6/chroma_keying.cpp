#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int rgb2cb(int r, int g, int b){
    int cb;
    cb = (int)round(128+ (-0.168736*r) - 0.331264*g + 0.5*b);
    return cb;
}

int rgb2cr(int r, int g, int b){
    int cr;
    cr = (int) round(128 + 0.5*r - 0.418688*g - 0.081312*b);
    return cr;
}

double colorclose(int Cb_p, int Cr_p, int Cb_key, int Cr_key, int tola, int tolb){
    double temp = sqrt((Cb_key - Cb_p)*(Cb_key - Cb_p)+(Cr_key - Cr_p)*(Cr_key -Cr_p));
    if(temp < tola) 
        return 0.0;
    if(temp < tolb)
        return ((temp - tola)/(tolb - tola));
    return 1.0;
}

int cb_key = -1;
int cr_key = -1;
int b_key = -1;
int g_key = -1;
int r_key = -1;
Mat frame, hsv, mask;

static void onMouse(int event, int x, int y, int flags, void* data){
    if(event == EVENT_LBUTTONDOWN){
        printf("location x:%d, y:%d\n", x, y);
        Rect box = Rect(x - 40, y -40, 80, 80);
        //out of bounding
        if(box.x < 0 || box.y < 0 ||(box.x + box.width)>= frame.cols|| (box.y+box.height)>=frame.rows){
            return;
        }
        //生成色键
        Mat roi = frame(box);
        Scalar m = mean(roi);
        b_key = (int)m[0];
        g_key = (int)m[1];
        r_key = (int)m[2];
        cb_key = rgb2cb(r_key, g_key, b_key);
        cr_key = rgb2cr(r_key, g_key, b_key);
        printf("RGB key: %d, %d, %d, Cb %d, Cr %d \n", r_key, g_key, b_key, cb_key, cr_key);
    }
}

int main(){
    Mat background = imread("../back_ground.jpg");
    VideoCapture capture;
    capture.open("../greenscreen-asteroid.mp4");
    if(!capture.isOpened()){
        printf("could not load video file...");
        return -1;
    }

    namedWindow("frame", WINDOW_AUTOSIZE);
    setMouseCallback("frame", onMouse, NULL);
    int type = -1;
    while(true){
        bool ret = capture.read(frame);
        if(!ret) break;
        if(type == 1){
            //绿幕抠图
            cvtColor(frame, hsv, COLOR_BGR2HSV);
            inRange(hsv, Scalar(35,43,46), Scalar(77,255,255), mask);
            Mat se = getStructuringElement(MORPH_RECT, Size(5,5), Point(-1,-1));
            morphologyEx(mask, mask, MORPH_CLOSE, se);
            background.copyTo(frame, mask);
        }

        else if (type == 2 && cb_key >0){ //press key 2
            cvtColor(frame, hsv, COLOR_BGR2HSV);
            inRange(hsv, Scalar(35,43,46), Scalar(77, 255, 255), mask);
            Mat se = getStructuringElement(MORPH_RECT, Size(5,5), Point(-1,-1));
            morphologyEx(mask,mask, MORPH_CLOSE, se);
            bitwise_not(mask, mask);
            
            //色键合成
            for(int row = 0; row < mask.rows; row++){
                for(int col = 0; col < mask.cols; col++){
                    int pv = mask.at<uchar>(row, col);
                    if(pv > 0){
                        int b1 = frame.at<Vec3b>(row, col)[0];
                        int g1 = frame.at<Vec3b>(row, col)[1];
                        int r1 = frame.at<Vec3b>(row, col)[2];
                        int cr = rgb2cr(r1,g1,b1);
                        int cb = rgb2cb(r1,g1,b1);

                        int b2 = background.at<Vec3b>(row, col)[0];
                        int g2 = background.at<Vec3b>(row, col)[1];
                        int r2 = background.at<Vec3b>(row, col)[2];

                        double w = colorclose(cb, cr, cb_key, cr_key, 2, 50);
                        w = 1.0 -w;

                        frame.at<Vec3b>(row, col)[0] = std::max(b1 -w *b_key, 0.0) + w*b2;
                        frame.at<Vec3b>(row, col)[1] = std::max(g1 -w *g_key, 0.0) + w*g2;
                        frame.at<Vec3b>(row, col)[2] = std::max(r1 -w *r_key, 0.0) + w*r2;
                    }
                }
            }

        }
    }

    waitKey(0);
    return 0;
}