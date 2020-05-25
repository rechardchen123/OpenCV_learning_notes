#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    //read the images
    std::vector<Mat> imgs;
    string pattern = "../scene/*.jpg";
    vector<string> picPaths;
    glob(pattern, picPaths, false);
    size_t size = picPaths.size();

    if (size > 1)
    {

        cout << "find " << size << " images!" << endl;
        for (int i = 0; i < size; i++)
        {
            Mat img = imread(picPaths[i]);
            imgs.push_back(img);
        }

        Mat result1, result2, result3;
        Stitcher::Mode mode = Stitcher::PANORAMA;
        Ptr<Stitcher> stitcher = Stitcher::create(mode);

        //拼接方式-多通道融合
        auto blender = detail::Blender::createDefault(detail::Blender::MULTI_BAND);
        stitcher->setBlender(blender);

        //拼接
        auto affine_warper = makePtr<cv::AffineWarper>();
        stitcher->setWarper(affine_warper);
        Stitcher::Status status = stitcher->stitch(imgs, result1);

        //平面曲翘连接
        auto plane_warper = makePtr<cv::PlaneWarper>();
        stitcher->setWarper(plane_warper);
        status = stitcher->stitch(imgs, result2);

        //鱼眼拼接
        auto fisher_warper = makePtr<cv::FisheyeWarper>();
        stitcher->setWarper(fisher_warper);
        status = stitcher->stitch(imgs, result3);

        //检查返回
        if (status != Stitcher::OK)
        {
            cout << "Cannot stitch images, error code = " << int(status) << endl;
            return EXIT_FAILURE;
        }

        imwrite("../result1.jpg", result1);
        imwrite("../result2.jpg", result2);
        imwrite("../result3.jpg", result3);
    }

    else if (size == 1)
    {

        cout << "find 1 image!" << endl;
        imshow("img", imread(picPaths[0]));
        waitKey(0);
    }
    else
    {
        cout << "find 0 image !" << endl;
    }

    return 0;
}