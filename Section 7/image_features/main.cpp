#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void ORB_detect()
{
    Mat img = imread("../book.jpeg");
    namedWindow("input", WINDOW_NORMAL);
    imshow("input", img);
    //Convert to grayscale
    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    namedWindow("input1", WINDOW_NORMAL);
    imshow("input1", imgGray);

    //Initial ORB detector
    Ptr<ORB> orb = ORB::create();

    //find the keypoints with ORB
    vector<KeyPoint> kp;
    orb->detect(imgGray, kp, Mat());

    //compute the descriptions with ORB
    Mat des;
    orb->compute(imgGray, kp, des);

    //draw only keypoints location
    Mat img2;
    drawKeypoints(img, kp, img2, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    namedWindow("result", WINDOW_NORMAL);
    imshow("result", img2);
    waitKey(0);
}

void feature_match()
{
    Mat img1 = imread("../book.jpeg", IMREAD_GRAYSCALE);
    Mat img2 = imread("../book_scene.jpeg", IMREAD_GRAYSCALE);

    //INITIAL ORB detector
    Ptr<ORB> orb = ORB::create();

    //find the keypoints and descriptors with ORB
    vector<KeyPoint> kp1, kp2;
    Mat des1, des2;

    orb->detectAndCompute(img1, Mat(), kp1, des1);
    orb->detectAndCompute(img2, Mat(), kp2, des2);

    //create BFMatcher object
    BFMatcher bf(NORM_HAMMING, true);

    vector<DMatch> matches;
    bf.match(des1, des2, matches, Mat());

    //sort them in the order of their distance
    matches = vector<DMatch>(matches.begin(), matches.begin() + 10);
    Mat img3;
    vector<char> match_mask(matches.size(), 1);
    drawMatches(img1, kp1, img2, kp2, matches, img3, Scalar::all(-1), Scalar::all(-1), match_mask,
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("output", WINDOW_NORMAL);
    imshow("output", img3);
    waitKey(0);
}

void FLANN_match()
{
    Mat img1 = imread("../book.jpeg", IMREAD_GRAYSCALE);
    Mat img2 = imread("../book_scene.jpeg", IMREAD_GRAYSCALE);

    //INITIAL ORB detector
    Ptr<ORB> orb = ORB::create();

    //find the keypoints and descriptors with ORB
    vector<KeyPoint> kp1, kp2;
    Mat des1, des2;

    orb->detectAndCompute(img1, Mat(), kp1, des1);
    orb->detectAndCompute(img2, Mat(), kp2, des2);

    //create FLANN object
    FlannBasedMatcher matcher(new flann::LshIndexParams(6, 12, 1), new flann::SearchParams(50));
    std::vector<std::vector<DMatch>> matches_vec;
    matcher.knnMatch(des1, des2, matches_vec, 2);
    matches_vec = vector<vector<DMatch>>(matches_vec.begin(), matches_vec.begin() + 10);
    Mat img3;
    drawMatches(img1, kp1, img2, kp2, matches_vec, img3);
    namedWindow("flann_match", WINDOW_NORMAL);
    imshow("flann_match", img3);
    waitKey(0);
}

int main()
{
    //ORB_detect();
    //feature_match();
    FLANN_match();

    return 0;
}