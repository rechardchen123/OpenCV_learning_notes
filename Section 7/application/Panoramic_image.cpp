#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat im1 = imread("../scene/scene1.jpg");
    Mat im2 = imread("../scene/scene3.jpg");

    Mat im1Gray, im2Gray;
    cvtColor(im1, im1Gray, COLOR_BGR2GRAY);
    cvtColor(im2, im2Gray, COLOR_BGR2GRAY);

    //variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    int MAX_FEATURES = 500;
    float GOOD_MATCH_PERCENT = 0.15f;
    //detect ORB features and compute descriptors.
    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);

    //Match features
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    //sort match by score
    std::sort(matches.begin(), matches.end());

    //remove not so good matches
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    //draw top matches
    Mat imMatches;
    drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);
    imwrite("../panoramic_image.jpg", imMatches);

    std::vector<Point2f> points1, points2;
    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    //find homography
    Mat h = findHomography(points2, points1, RANSAC);

    //use homography to wrap image
    int im1Height = im1.rows;
    int im1Width = im1.cols;
    int im2Height = im2.rows;
    int im2Width = im2.cols;
    Mat im2Aligned;

    warpPerspective(im2, im2Aligned, h, Size(im2Width + im1Width, im2Height));
    namedWindow("result", WINDOW_NORMAL);
    imshow("result", im2Aligned);

    Mat stitchedImage = im2Aligned.clone();
    Rect roi(0, 0, im1.cols, im1.rows);
    im1.copyTo(stitchedImage(roi));
    imwrite("../result_aligned.jpg", stitchedImage);

    waitKey(0);
    return 0;
}