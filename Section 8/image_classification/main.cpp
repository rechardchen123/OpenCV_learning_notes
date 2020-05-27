#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include "cropEyeRegion.h"

using namespace std;
using namespace cv::ml;
using namespace cv;

#define FACE_DOWNSAMPLE_RATIO 1

void getFilenames(string dirName, vector<string> &imageFnames)
{
    glob(dirName, imageFnames, false);
}

void loadTrainTestLabel(string &pathName, vector<Mat> &trainImages, vector<Mat> &testImages, vector<int> &trainLabels,
                        vector<int> &testLabels, int classVal, float testFraction = 0.2)
{
    vector<string> imageFiles;

    getFilenames(pathName, imageFiles);

    int totalImages = imageFiles.size();
    int nTest = (int)(testFraction * totalImages);

    for (int counter = 0; counter < totalImages; counter++)
    {
        Mat image = imread(imageFiles[counter]);
        if (counter < nTest)
        {
            testImages.push_back(image);
            testLabels.push_back(classVal);
        }
        else
        {
            trainImages.push_back(image);
            trainLabels.push_back(classVal);
        }
    }
}

HOGDescriptor hog(
    Size(96, 32), //winSize
    Size(8, 8),   //blocksize
    Size(8, 8),   //blockStride,
    Size(4, 4),   //cellSize,
    9,            //nbins,
    0,            //derivAper,
    4,            //winSigma,
    HOGDescriptor::L2Hys,
    0.2, //L2HysThresh,
    1,   //gammal correction,
    64,  //nlevels=64
    1);  //signedGradient

void CreateHOG(vector<vector<float>> &HOG, vector<Mat> &Images)
{
    for (int y = 0; y < Images.size(); y++)
    {
        vector<float> descriptors;
        hog.compute(Images[y], descriptors);
        HOG.push_back(descriptors);
    }
}

void ConvertVectortoMatrix(vector<vector<float>> &HOG, Mat &samples)
{
    int descriptor_size = HOG[0].size();

    for (int i = 0; i < HOG.size(); i++)
    {
        for (int j = 0; j < descriptor_size; j++)
        {
            samples.at<float>(i, j) = HOG[i][j];
        }
    }
}

Ptr<SVM> svmInit(float C, float gamma)
{
    Ptr<SVM> svm = SVM::create();
    svm->setGamma(gamma);
    svm->setC(C);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);

    return svm;
}

void svmTrain(Ptr<SVM> svm, Mat &trainMat, vector<int> &trainLabels)
{
    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    svm->train(td);
    svm->save("../eyeGlassClassifierModel.yml");
}

void svmPredict(Ptr<SVM> svm, Mat &testResponse, Mat &testMat)
{
    svm->predict(testMat, testResponse);
}

void SVMevaluate(Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels)
{
    for (int i = 0; i < testResponse.rows; i++)
    {
        //cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if (testResponse.at<float>(i, 0) == testLabels[i])
            count = count + 1;
    }
    accuracy = (count / testResponse.rows) * 100;
}

void getSVMParams(Ptr<SVM> svm)
{
    cout << "Kernel type     : " << svm->getKernelType() << endl;
    cout << "Type            : " << svm->getType() << endl;
    cout << "C               : " << svm->getC() << endl;
    cout << "Degree          : " << svm->getDegree() << endl;
    cout << "Nu              : " << svm->getNu() << endl;
    cout << "Gamma           : " << svm->getGamma() << endl;
}

int main(int argc, char **argv)
{
    vector<Mat> trainImages;
    vector<Mat> testImages;
    vector<int> trainLabels;
    vector<int> testLabels;

    //path load
    string path1 = "C:/Users/ucesxc0/Desktop/OpenCV/section8/glassesDataset/cropped_withoutGlasses2";
    string path2 = "C:/Users/ucesxc0/Desktop/OpenCV/section8/glassesDataset/cropped_withGlasses2";

    //get the training and testing images for both classes
    loadTrainTestLabel(path1, trainImages, testImages, trainLabels, testLabels, 0);
    loadTrainTestLabel(path2, trainImages, testImages, trainLabels, testLabels, 1);

    ////////feature computation for the data  //////////////
    std::vector<std::vector<float>> trainHOG;
    std::vector<std::vector<float>> testHOG;
    CreateHOG(trainHOG, trainImages);
    CreateHOG(testHOG, testImages);

    int descriptor_size = trainHOG[0].size();
    cout << "Descriptor size: " << descriptor_size << endl;

    //conver hog data to features recognized by SVM model
    Mat trainMat(trainHOG.size(), descriptor_size, CV_32FC1);
    Mat testMat(testHOG.size(), descriptor_size, CV_32FC1);

    ConvertVectortoMatrix(trainHOG, trainMat);
    ConvertVectortoMatrix(testHOG, testMat);

    float C = 2.5, gamma = 0.02;

    Mat testResponse;
    Ptr<SVM> model = svmInit(C, gamma);

    ///////////  SVM Training  ////////////////
    svmTrain(model, trainMat, trainLabels);

    ///////////  SVM Testing  ////////////////
    svmPredict(model, testResponse, testMat);

    ////////////// Find Accuracy   ///////////
    float count = 0;
    float accuracy = 0;
    getSVMParams(model);
    SVMevaluate(testResponse, count, accuracy, testLabels);
    cout << "Percentage Accuracy : " << accuracy << endl;

    /////////// Testing a single image   ////////////////////
    string filename = "C:/Users/ucesxc0/Desktop/OpenCV/section8/glassesDataset/glasses_3.jpg";
    if (argc == 2)
    {
        filename = argv[1];
    }

    // Read the image
    Mat testImage = imread(filename);

    // Get the eye region
    Mat cropped = getCroppedEyeRegion(testImage);

    // Create vector of images for testing
    vector<Mat> testImageArray;
    testImageArray.push_back(cropped);

    // Compute HOG descriptors
    std::vector<std::vector<float>> testHOGArray;
    CreateHOG(testHOGArray, testImageArray);

    // Convert the descriptors to Mat
    Mat testSample(testHOGArray.size(), descriptor_size, CV_32FC1);
    ConvertVectortoMatrix(testHOGArray, testSample);

    // We will load the model again and test the model
    // This is just to explain how to load an SVM model
    // You can use the model directly too

    Ptr<SVM> savedModel = StatModel::load<SVM>("../eyeGlassClassifierModel.yml");
    Mat pred;
    svmPredict(savedModel, pred, testSample);

    cout << "\n Prediction : " << pred.at<float>(0, 0) << endl;
    imshow("Test Image", testImage);
    imshow("cropped", cropped);
    waitKey(0);
    destroyAllWindows();

    return 0;
}