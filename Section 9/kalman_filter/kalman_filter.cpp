/*
The Kamal filter has been used to track the pedestrain in the video.
*/

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 矩形框比较函数
bool rectAreaComparator(Rect &r1, Rect &r2)
{
  return r1.area() < r2.area();
}

// 显示颜色设置
Scalar blue(255,128,0);
Scalar red(64,64,255);

int main(int, char**)
{

  // 初始化行人检测的HOG算子
  HOGDescriptor hog(
                    Size(64, 128), //winSize
                    Size(16, 16),  //blocksize
                    Size(8, 8),    //blockStride,
                    Size(8, 8),    //cellSize,
                    9,
                    0);           
  // 设置行人检测算子
  vector< float > svmDetector = hog.getDefaultPeopleDetector();
  hog.setSVMDetector(svmDetector);
  float hitThreshold = 0.0;
  Size winStride = Size(8, 8);
  Size padding = Size(32, 32);
  float scale = 1.05;
  float finalThreshold = 1;
  bool useMeanshiftGrouping = 0;
  vector<double> weights;

  // 存放目标矩形框的vector
  std::vector<Rect> objects;

  // 读入视频
  VideoCapture cap("../boy-walking.mp4");

  // 检查视频是否读取成功
  if (!cap.isOpened())
  {
    cerr << "Unable to read video" << endl;
    return EXIT_FAILURE;
  }

  // 图像变量
  Mat frame, frameDisplayDetection, frameDisplay, output;

  // Kalman滤波器数据类型
  int type = CV_32F;

  // 初始化Kalman滤波器
  // 状态变量 (x, y, width, vx, vy, vw)
  // 测量值 (x, y, width)
  // 由于Height = 2 x width，所以height不作为变量
  KalmanFilter KF(6, 3, 0, type);

  /*
   由于
   x = x + vx * dt
   y = y + vy * dt
   w = y + vw * dt

   vx = vx
   vy = vy
   vw = vw

   状态转移矩阵为
   [
     1, 0, 0, dt, 0,  0,
     0, 1, 0, 0,  dt, 0,
     0, 0, 1, 0,  0,  dt,
     0, 0, 0, 1,  0,  0,
     0, 0, 0, 0,  1,  0,
     0, 0, 0, dt, 0,  1
   ]
   */
  setIdentity(KF.transitionMatrix);

  /*
   由于只测量x, y和w, 观测矩阵为
   [
    1, 0, 0, 0, 0,  0,
    0, 1, 0, 0, 0,  0,
    0, 0, 1, 0, 0,  0,
   ]
  */
  setIdentity(KF.measurementMatrix);

  // 存放x, y和w的Mat
  Mat measurement = Mat::zeros(3, 1, type);

  // 存放检测的目标和跟踪的目标的Rect
  Rect objectTracked, objectDetected;

  // 存放预测和更新值的Mat
  Mat updatedMeasurement, predictedMeasurement;

  // 是否已更新的标志
  bool measurementWasUpdated = false;

  // 时间
  double ticks, preTicks;

  // 读入视频帧并进行行人检测，直到能检测出行人
  while(cap.read(frame))
  {

    // 行人检测
    hog.detectMultiScale(frame, objects, weights, hitThreshold, winStride, padding,
                         scale, finalThreshold, useMeanshiftGrouping);

    // 更新计时器
    ticks = (double) cv::getTickCount();


    if(objects.size() > 0 )
    {
      // 检测到的第一个目标
      objectDetected = *std::max_element(objects.begin(), objects.end(), rectAreaComparator);

      // 更新测量值
      measurement.at<float>(0) = objectDetected.x;
      measurement.at<float>(1) = objectDetected.y;
      measurement.at<float>(2) = objectDetected.width;

      // 更新状态变量，将x, y, w设置为测量值.
      // 由于不知道速度，设vx = vy = vw 
      KF.statePost.at<float>(0) = measurement.at<float>(0);
      KF.statePost.at<float>(1) = measurement.at<float>(1);
      KF.statePost.at<float>(2) = measurement.at<float>(2);
      KF.statePost.at<float>(3) = 0;
      KF.statePost.at<float>(4) = 0;
      KF.statePost.at<float>(5) = 0;

      // 设置协方差矩阵的对角线元素值
      // 过程噪声协方差为Q
      setIdentity(KF.processNoiseCov, Scalar::all(1e-2));

      // 测量噪声协方差为R
      setIdentity(KF.measurementNoiseCov, Scalar::all(1e-2));
      break;
    }
  }

  // 转移矩阵的dt
  double dt = 0;

  // 产生随机数随机选取帧进行更新
  RNG rng( 0xFFFFFFFF );

  // 循环所有视频帧
  while(cap.read(frame))
  {
    // 显示跟踪结果的图像
    frameDisplay = frame.clone();

    // 显示检测结果的图像
    frameDisplayDetection = frame.clone();

    // 更新转移矩阵的dt
    // dt = 间隔时间

    preTicks = ticks;
    ticks = (double) cv::getTickCount();
    dt = (ticks - preTicks) / cv::getTickFrequency();

    KF.transitionMatrix.at<float>(3) = dt;
    KF.transitionMatrix.at<float>(10) = dt;
    KF.transitionMatrix.at<float>(17) = dt;

    // Kalman滤波的预测
    predictedMeasurement = KF.predict();


    // 清除前一帧中检测到的目标
    objects.clear();

    // 在当前帧中进行目标检测
    hog.detectMultiScale(frame, objects, weights, hitThreshold, winStride, padding,
                         scale, finalThreshold, useMeanshiftGrouping);

    // 找出最大的目标
    objectDetected = *std::max_element(objects.begin(), objects.end(), rectAreaComparator);

    // 显示结果
    rectangle(frameDisplayDetection, objectDetected, red, 2, 4);

    // 只有15%的时间执行更新
    // 视频帧是随机选择的
    bool update = rng.uniform( 0.0, 1.0) < 0.15;

    if (update)
    {
      // Kalman滤波的更新
      if(objects.size() > 0 )
      {
        // 从检测的矩形框复制x, y, w
        measurement.at<float>(0) = objectDetected.x;
        measurement.at<float>(1) = objectDetected.y;
        measurement.at<float>(2) = objectDetected.width;

        // 执行Kalman更新步骤
        updatedMeasurement = KF.correct(measurement);
        measurementWasUpdated = true;
      }
      else
      {
        // 如果没有检测到目标则不更新
        measurementWasUpdated = false;
      }

    }
    else
    {
      // 不更新
      measurementWasUpdated = false;

    }

    if(measurementWasUpdated)
    {
      // 如果更新，使用更新的值
      objectTracked = Rect(updatedMeasurement.at<float>(0), updatedMeasurement.at<float>(1),updatedMeasurement.at<float>(2),2 * updatedMeasurement.at<float>(2));
    }
    else
    {
      // 如果未更新，使用预测的值
      objectTracked = Rect(predictedMeasurement.at<float>(0), predictedMeasurement.at<float>(1),predictedMeasurement.at<float>(2),2 * predictedMeasurement.at<float>(2));
    }

    // 绘制跟踪的目标
    rectangle(frameDisplay, objectTracked, blue, 2, 4);

    // 文字显示跟踪或检测的结果
    putText(frameDisplay,"Tracking", Point(20,40),FONT_HERSHEY_SIMPLEX, 0.75, blue, 2);
    putText(frameDisplayDetection,"Detection", Point(20,40),FONT_HERSHEY_SIMPLEX, 0.75, red, 2);

    // 将检测结果图像和跟踪结果图像竖直拼为一幅图像存放进output
    vconcat(frameDisplayDetection, frameDisplay, output);

    // 显示结果
    imshow("object Tracker", output);
    int key = waitKey(5);
    // 按ESC退出
    if ( key == 27 )
    {
      break;
    }

  }
  return EXIT_SUCCESS;
}
