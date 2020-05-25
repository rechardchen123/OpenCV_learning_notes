#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void align_channels()
{
    Mat image = imread("../*.jpg"); //input the image that you want to process....
    imshow("input", image);
    vector<Mat> channels;
    split(image, channels);
    Mat blue = channels[0];
    Mat green = channels[1];
    Mat red = channels[2];
    int MAX_FEATURES = 1000;
    float GOOD_MATCH_PERCENT = 0.15f;
    std::vector<KeyPoint> keypointsBlue, keypointsGreen, keypointsRed;
    Mat descriptorsBlue, descriptorsGreen, descriptorsRed;

    Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(blue, Mat(), keypointsBlue, descriptorsBlue);
    orb->detectAndCompute(green, Mat(), keypointsGreen, descriptorsGreen);
    orb->detectAndCompute(red, Mat(), keypointsRed, descriptorsRed);

    std::vector<DMatch> matchesBlueGreen;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptorsBlue, descriptorsGreen, matchesBlueGreen, Mat());

    //sort matches by scores
    std::sort(matchesBlueGreen.begin(), matchesBlueGreen.end());

    //remove not so good matches
    int numGoodMatches = matchesBlueGreen.size() * GOOD_MATCH_PERCENT;
    matchesBlueGreen.erase(matchesBlueGreen.begin() + numGoodMatches, matchesBlueGreen.end());

    //draw top matches
    Mat imMatchesBlueGreen;
    drawMatches(blue, keypointsBlue, green, keypointsGreen, matchesBlueGreen, imMatchesBlueGreen);

    std::vector<DMatch> matchesRedGreen;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptorsRed, descriptorsGreen, matchesRedGreen, Mat());

    std::sort(matchesRedGreen.begin(), matchesRedGreen.end());

    int numGoodMatches = matchesRedGreen.size() * GOOD_MATCH_PERCENT;
    matchesRedGreen.erase(matchesRedGreen.begin() + numGoodMatches, matchesRedGreen.end());

    //Draw top matches
    Mat imMatchesRedGreen;
    drawMatches(red, keypointsRed, green, keypointsGreen, matchesRedGreen, imMatchesRedGreen);

    std::vector<Point2f> pointsBlue, pointsGreen;

    for (size_t i = 0; i < matchesBlueGreen.size(); i++)
    {
        pointsBlue.push_back(keypointsBlue[matchesBlueGreen[i].queryIdx].pt);
        pointsGreen.push_back(keypointsGreen[matchesBlueGreen[i].trainIdx].pt);
    }

    //find homography
    Mat hBlueGreen = findHomography(pointsBlue, pointsGreen, RANSAC);

    pointsGreen.clear();

    std::vector<Point2f> pointsRed;
    for (size_t i = 0; i < matchesRedGreen.size(); i++)
    {
        pointsRed.push_back(keypointsRed[matchesRedGreen[i].queryIdx].pt);
        pointsGreen.push_back(keypointsGreen[matchesRedGreen[i].trainIdx].pt);
    }
    Mat hRedGreen = findHomography(pointsRed, pointsGreen, RANSAC);

    Mat blueWarped, redWarped;
    warpPerspective(blue, blueWarped, hBlueGreen, green.size());
    warpPerspective(red, redWarped, hRedGreen, green.size());

    Mat colorImage;
    vector<Mat> colorImageChannels{blueWarped, green, redWarped};
    merge(colorImageChannels, colorImage);
    medianBlur(colorImage, colorImage, 3);
    imshow("algined image", colorImage);

    waitKey(0);
    destroyAllWindows();
}

int main()
{
    align_channels();
    return 0;
}