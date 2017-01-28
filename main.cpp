#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/gpu/gpu.hpp"

#include <iostream>
#include <stdlib.h>


using namespace std;
using namespace cv;
int run_video(char* file);
/*void on_lRed_thresh_trackbar(int, void *);
void on_uRed_thresh_trackbar(int, void *);
void on_lGreen_thresh_trackbar(int, void *);
void on_uGreen_thresh_trackbar(int, void *);
void on_lBlue_thresh_trackbar(int, void *);
void on_uBlue_thresh_trackbar(int, void *);*/
//RGB RANGE
//int lRed = 0, lGreen = 215, lBlue = 0, uRed = 179, uGreen = 255, uBlue = 241;
//HSV RANGE
int lH = 48, lS = 80, lV = 210, uH = 80, uS = 246, uV = 255;

int main(int v,char* argv[]){
  //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"); //open the default camera

  cout << argv[1] << "\n";
  namedWindow("original", WINDOW_NORMAL);
  namedWindow("filtered", WINDOW_NORMAL);
  namedWindow("hsv", WINDOW_NORMAL);

  //createTrackbar("Low R","filtered", &lRed, 255, on_lRed_thresh_trackbar);
  //createTrackbar("High R","filtered", &uRed, 255, on_uRed_thresh_trackbar);
  //createTrackbar("Low G","filtered", &lGreen, 255, on_lGreen_thresh_trackbar);
  //createTrackbar("High G","filtered", &uGreen, 255, on_uGreen_thresh_trackbar);
  //createTrackbar("Low B","filtered", &lBlue, 255, on_lBlue_thresh_trackbar);
  //createTrackbar("High B","filtered", &uBlue, 255, on_uBlue_thresh_trackbar);

  while(1) { run_video(argv[1]); }

  return 0;
}
int run_video(char* file)
{
  Mat hsv, hsv_blur, filter;
  Mat map1, map2, cameraMatrix, distCoeffs, imageSize;
  Size imageSize;
  VideoCapture cap;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
  cap.open(0);
  if(!cap.isOpened()) { // check if we succeeded
    cerr << "Fail to open camera " << endl;
    return 0;
  }
  cameraMatrix = Mat::eye(3, 3, CV_64F);
  initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize, CV_16SC2, map1, map2);
  for(;;)
  {
    Mat frame;
    cap.read(frame); // get a new frame from camera
    Mat temp = frame.clone();
    undistort(temp, frame, cameraMatrix, distCoeffs);
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    //inRange(hsv_img, Scalar(lBlue, lGreen, lRed), Scalar(uBlue, uGreen, uRed), greenLowerHue);
    //GaussianBlur(hsv, hsv_blur, Size(3, 3), 0, 0);
    inRange(hsv, Scalar(lH, lS, lV), Scalar(uH, uS, uV), filter);
    imshow("original", frame);
		imshow("hsv", hsv);
    imshow("filtered", filter);
    waitKey(1);
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  cap.release();
  return 1;
}
/*void on_lRed_thresh_trackbar(int, void *)
{
    lRed = min(uRed-1, lRed);
    setTrackbarPos("Low R","filtered", lRed);
}
void on_uRed_thresh_trackbar(int, void *)
{
    uRed = max(uRed, lRed+1);
    setTrackbarPos("High R", "filtered", uRed);
}
void on_lGreen_thresh_trackbar(int, void *)
{
    lGreen = min(uGreen-1, lGreen);
    setTrackbarPos("Low G","filtered", lGreen);
}
void on_uGreen_thresh_trackbar(int, void *)
{
    uGreen = max(uGreen, lGreen+1);
    setTrackbarPos("High G", "filtered", uGreen);
}
void on_lBlue_thresh_trackbar(int, void *)
{
    lBlue= min(uBlue-1, lBlue);
    setTrackbarPos("Low B","filtered", lBlue);
}
void on_uBlue_thresh_trackbar(int, void *)
{
    uBlue = max(uBlue, lBlue+1);
    setTrackbarPos("High B", "filtered", uBlue);
}*/
/*#include <opencv2/opencv.hpp>
using namespace cv;
int main()
{
  string gst = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format(string)BGR ! appsink";
  cv::Mat img;
  VideoCapture input(1);
  namedWindow("img", 1);
  while(true){
    Mat frame;
    input.read(frame);
    imshow("img", frame);
    if(waitKey(30)>=0){
      break;
    }
  }
}*/

