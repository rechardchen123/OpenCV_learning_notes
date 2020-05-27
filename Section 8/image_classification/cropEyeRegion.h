#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat getCroppedEyeRegion(Mat targetImage)
{
    String faceCascadePath = "C:/Users/ucesxc0/Downloads/opencv-4.2.0/minGW-build/install/etc/haarcascades/haarcascade_frontalface_default.xml";

    CascadeClassifier faceCascade;
    if (!faceCascade.load(faceCascadePath))
    {
        printf("--(!)Error loading face cascade\n");
    };

    Mat targetImageGray;

    cvtColor(targetImage, targetImageGray, COLOR_BGR2GRAY);

    vector<Rect> faces;
    faceCascade.detectMultiScale(targetImageGray, faces, 1.3, 5);

    int x = faces[0].x;
    int y = faces[0].y;
    int w = faces[0].width;
    int h = faces[0].height;

    Mat face_roi = targetImage(Range(y, y + h), Range(x, x + w));
    imwrite("face_roi.png", face_roi);
    int face_height, face_width;
    face_height = face_roi.size().height;
    face_width = face_roi.size().width;

    // Apply a heuristic formula for getting the eye region from face
    int eyeTop = (int)(1.0 / 6.0 * face_height);
    int eyeBottom = (int)(3.0 / 6.0 * face_height);

    cout << "Eye Height between : " << eyeTop << "," << eyeBottom << endl;

    Mat eye_roi = face_roi(Range(eyeTop, eyeBottom), Range(0, face_width));

    // Resize the eye region to a fixed size of 96x32
    Mat cropped;
    resize(eye_roi, cropped, Size(96, 32));

    return cropped;
}
