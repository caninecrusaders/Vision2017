#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/gpu/gpu.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>


using namespace std;
using namespace cv;
int run_video(char* file);
void on_lH_thresh_trackbar(int, void *);
void on_hH_thresh_trackbar(int, void *);
void on_lS_thresh_trackbar(int, void *);
void on_hS_thresh_trackbar(int, void *);
void on_lV_thresh_trackbar(int, void *);
void on_hV_thresh_trackbar(int, void *);
void readCalibration(string filename, Mat& image, Mat& coeffs);
void addTrackBars(string window);
void computeFocalLength(Mat &src);
void computeDistance(Mat &src);
void applyFilter(Mat &src, Mat &dest);
int findHeightOfLargestContour(Mat &src);
Mat cameraMatrix, distCoeffs;
int lV = 80, lS = 220, lH = 130, hV = 130, hS = 255, hH = 255;
int main(int v,char* argv[]){
  
  cout << argv[1] << "\n";
  namedWindow("original", WINDOW_NORMAL);
  namedWindow("filtered", WINDOW_NORMAL);
  addTrackBars("filtered");
  readCalibration(argv[1], cameraMatrix, distCoeffs);
  run_video(argv[1]);

  return 0;
}
int run_video(char* file)
{
  VideoCapture cap;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  cap.open(1);
  if(!cap.isOpened()) { // check if we succeeded
    cerr << "Fail to open camera " << endl;
    return 0;
  }
  for(;;)
  {
  	Mat filter, frame, smallerFrame;
    cap.read(frame); // get a new frame from camera
    Mat temp = frame.clone();
    undistort(temp, frame, cameraMatrix, distCoeffs);
    resize(frame, smallerFrame, Size(480,320));
    computeFocalLength(frame);		//Computes and displays focal length for camera
    //computeDistance(frame);		//Computes and displays distance in inches
    applyFilter(smallerFrame, filter);
    imshow("original", frame); 
    imshow("filtered", filter);
    waitKey(1);
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  cap.release();
  return 1;
}
void on_lH_thresh_trackbar(int, void *)
{
    lH = min(hH-1, lH);
    setTrackbarPos("Low H","filtered", lH);
}
void on_hH_thresh_trackbar(int, void *)
{
    hH = max(hH, lH+1);
    setTrackbarPos("High H", "filtered", hH);
}
void on_lS_thresh_trackbar(int, void *)
{
    lS = min(hS-1, lS);
    setTrackbarPos("Low S","filtered", lS);
}
void on_hS_thresh_trackbar(int, void *)
{
    hS = max(hS, lS+1);
    setTrackbarPos("High S", "filtered", hS);
}
void on_lV_thresh_trackbar(int, void *)
{
    lV= min(hV-1, lV);
    setTrackbarPos("Low V","filtered", lV);
}
void on_hV_thresh_trackbar(int, void *)
{
    hV = max(hV, lV+1);
    setTrackbarPos("High V", "filtered", hV);
}
void computeFocalLength(Mat &src)
{
    //Known height is the height in inches of the object you are finding. Known distance is distance from camera to object
    double focalLength = 0, knownHeight = 8.5, knownDistance = 24.0;
  	int height = findHeightOfLargestContour(src);
    focalLength = (double) knownDistance * height / knownHeight;
    std::stringstream s;
    s << focalLength << "(focalLength in px)";
    putText(src, s.str(), Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0));
    imshow("focalLength", src);
}
void computeDistance(Mat &src)
{

    //Known height is the height in inches of the object you are finding. Known distance is distance from camera to object
    double focalLength = 0, knownHeight = 8.5, distance = 0;
  	int height = findHeightOfLargestContour(src);
  	distance = focalLength * knownHeight / height;
    std::stringstream s;
    s << distance << "(distance in IN)";
    putText(src, s.str(), Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0));
    imshow("distance", src);
    
}
int findHeightOfLargestContour(Mat &src)
{
		Rect bounding_rect;
		int largest_area = 0, largest_contour_index=0;
		vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;
		Mat edged;
		cv::Canny(src, edged, 35, 125, 3);
    cv::findContours(edged, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (size_t i=0; i<contours.size(); i++)
    {
        double area = contourArea ( contours[i]);
        if (area > largest_area)
        {
            largest_area = area;
            largest_contour_index = i;
            bounding_rect = boundingRect( contours[i]);
        }
    }
    drawContours(src, contours, largest_contour_index, Scalar( 0, 255, 0 ), 2);
    return bounding_rect.height;
}
void addTrackBars(string window)
{
	createTrackbar("Low H", window, &lH, 255, on_lH_thresh_trackbar);
  createTrackbar("High H", window, &hH, 255, on_hH_thresh_trackbar);
  createTrackbar("Low S", window, &lS, 255, on_lS_thresh_trackbar);
  createTrackbar("High S", window, &hS, 255, on_hS_thresh_trackbar);
  createTrackbar("Low V", window, &lV, 255, on_lV_thresh_trackbar);
  createTrackbar("High V", window, &hV, 255, on_hV_thresh_trackbar);

}
void readCalibration(string filename, Mat& image, Mat& coeffs){
    cout << endl << "Reading: " << filename << endl;
    FileStorage fs;
    fs.open(filename, FileStorage::READ);

    if (!fs.isOpened())
    {
        cerr << "Failed to open " << filename << endl;
        return;
    }

    fs["camera_matrix"] >> image;                                      // Read cv::Mat
    fs["distortion_coefficients"] >> coeffs;
    cout << endl << "camera_matrix = " << image << endl;
    cout << "distortion_coeffs = " << coeffs << endl << endl;
}
void applyFilter(Mat &src, Mat &dest)
{
		Mat hsv;
 		cvtColor(src, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(lV, lS, lH), Scalar(hV, hS, hH), dest);
}

