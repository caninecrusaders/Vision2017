#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
/* #include "opencv2/gpu/gpu.hpp" */

#include <iostream>
#include <string>
#include <stdlib.h>


using namespace std;
using namespace cv;
int run_video(char* file);
void on_lRed_thresh_trackbar(int, void *);
void on_uRed_thresh_trackbar(int, void *);
void on_lGreen_thresh_trackbar(int, void *);
void on_uGreen_thresh_trackbar(int, void *);
void on_lBlue_thresh_trackbar(int, void *);
void on_uBlue_thresh_trackbar(int, void *);
void readCalibration(string filename, Mat& image, Mat& coeffs);
//RGB RANGE
//int lRed = 0, lGreen = 215, lBlue = 0, uRed = 179, uGreen = 255, uBlue = 241;
//HSV RANGE
int lBlue = 80, lGreen = 220, lRed = 130, uBlue = 130, uGreen = 255, uRed = 255;
/* int lH = 48, lS = 80, lV = 210, uH = 80, uS = 246, uV = 255; */

int main(int v,char* argv[]){
  //VideoCapture cap("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"); //open the default camera

  cout << argv[1] << "\n";

  /* namedWindow("original", WINDOW_NORMAL); */
  namedWindow("filtered", WINDOW_NORMAL);
  /* namedWindow("hsv", WINDOW_NORMAL); */

  createTrackbar("Low R","filtered", &lRed, 255, on_lRed_thresh_trackbar);
  createTrackbar("High R","filtered", &uRed, 255, on_uRed_thresh_trackbar);
  createTrackbar("Low G","filtered", &lGreen, 255, on_lGreen_thresh_trackbar);
  createTrackbar("High G","filtered", &uGreen, 255, on_uGreen_thresh_trackbar);
  createTrackbar("Low B","filtered", &lBlue, 255, on_lBlue_thresh_trackbar);
  createTrackbar("High B","filtered", &uBlue, 255, on_uBlue_thresh_trackbar);

  Mat imageMatrix, distortCoeffs;
  readCalibration(argv[1], &imageMatrix, &distortCoeffs);

  while(1) { run_video(argv[1]); }

  return 0;
}
int run_video(char* file)
{
  Mat hsv, hsv_blur, filter, edged;
  Mat map1, map2, cameraMatrix, distCoeffs, imageSize;
  int largest_area = 0, largest_contour_index=0;
  double focalLength = 1089.88;//1084.36;
  double distance;
  Rect bounding_rect;
  Size imageSize;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  VideoCapture cap;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
  cap.open(0);
  //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('A','V','C',1));
  //cap.open(0);
  if(!cap.isOpened()) { // check if we succeeded
    cerr << "Fail to open camera " << endl;
    return 0;
  }
  cameraMatrix = Mat::eye(3, 3, CV_64F);
  initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0), imageSize, CV_16SC2, map1, map2);
  for(;;)
  {
    Mat frame, smallerframe;
    cap.read(frame); // get a new frame from camera
    Mat temp = frame.clone();
    undistort(temp, frame, cameraMatrix, distCoeffs);
    resize(frame, smallerframe, Size(480,320));
    cvtColor(smallerframe, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(lBlue, lGreen, lRed), Scalar(uBlue, uGreen, uRed), filter);
    //GaussianBlur(hsv, hsv_blur, Size(3, 3), 0, 0);
    /* inRange(hsv, Scalar(lH, lS, lV), Scalar(uH, uS, uV), filter); */
		/* imshow("hsv", hsv); */
    /* cv::Canny(filter, edged, 35, 125, 3); */
    /* cv::findContours(edged, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); */
    /* for (size_t i=0; i<contours.size(); i++) */
    /* { */
    /*     double area = contourArea ( contours[i]); */
    /*     if (area > largest_area) */
    /*     { */
    /*         largest_area = area; */
    /*         largest_contour_index = i; */
    /*         bounding_rect = boundingRect( contours[i]); */
    /*     } */
    /* } */
    /* drawContours(filter, contours, largest_contour_index, Scalar( 0, 255, 0 ), 2); */
    /* distance = focalLength * 4.0 / bounding_rect.height; */
    /* std::string title = std::to_string(distance) + "(in)"; */
    /* putText(frame, title, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0)); */
    cout<<"r"<<lRed<<" "<<uRed<<"b"<<lBlue<<" "<<uBlue<<"g"<<lGreen<<" "<<uGreen<<endl;
    largest_contour_index = 0;
    largest_area = 0;
    /* imshow("original", frame); */
    imshow("filtered", filter);
    waitKey(1);
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  cap.release();
  return 1;
}
void on_lRed_thresh_trackbar(int, void *)
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
}
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
void readCalibration(string filename, Mat& image, Mat& coeffs){
    cout << endl << "Reading: " << filename << endl;
    FileStorage fs;
    fs.open(filename, FileStorage::READ);

    int itNr;
    //fs["iterationNr"] >> itNr;
    itNr = (int) fs["iterationNr"];
    cout << itNr;
    if (!fs.isOpened())
    {
        cerr << "Failed to open " << filename << endl;
        help(av);
        return 1;
    }

    FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
    for (; it != it_end; ++it)
        cout << (string)*it << endl;

    fs["image_matrix"] >> image;                                      // Read cv::Mat
    fs["distortion_coefficients"] >> coeffs;
    cout << endl
        << "image_matrix = " << image << endl;
    cout << "distortion_coefficients = " << distortion_coefficients << endl << endl;
}

