#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    //load image
    Mat imReference = imread("../form.jpg");
    namedWindow("reference", WINDOW_NORMAL);
    imshow("reference", imReference);

    Mat img = imread("../scanned-form.jpg");
    namedWindow("input", WINDOW_NORMAL);
    imshow("input", img);

    //feature detection
    int MAX_FEATURES = 500;
    float GOOD_MATCH_PERCENT = 0.15f;

    //convert images to grayscales
    Mat im1Gray, img2Gray;
    cvtColor(img, im1Gray, COLOR_BGR2GRAY);
    cvtColor(imReference, img2Gray, COLOR_BGR2GRAY);

    //variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    //Detect ORB features and compute descriptors
    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2Gray, Mat(), keypoints2, descriptors2);

    //match features
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    //Sort matches by score
    std::sort(matches.begin(), matches.end());
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    //draw top matches
    Mat imMatches;
    drawMatches(img, keypoints1, imReference, keypoints2, matches, imMatches);
    imwrite("../matches.jpg", imMatches);

    //Extract location of good matches
    std::vector<Point2f> points1, points2;
    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    //find homography
    Mat h = findHomography(points1, points2, RANSAC);

    //use homography to warp image
    Mat img1Reg;
    warpPerspective(img, img1Reg, h, imReference.size());
    namedWindow("result", WINDOW_NORMAL);
    imshow("result", img1Reg);

    waitKey(0);
    return 0;
}