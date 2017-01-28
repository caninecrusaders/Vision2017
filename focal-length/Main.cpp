#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string>
using namespace cv; 
using namespace std;
int main() {
    int largest_area=0;
    int largest_contour_index=0;
    double focalLength = 0;//1084.36;
    double distance;
    Rect bounding_rect;    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat frame, grey, blur, edged;
    cv::VideoCapture cap(0);
    while(true){
        cap.read(frame);
        cv::cvtColor(frame, grey, COLOR_BGR2GRAY);
        cv::GaussianBlur(grey, blur, Size(5, 5), 0, 0);
        cv::Canny(blur, edged, 35, 125, 3);
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
        drawContours( frame, contours, largest_contour_index, Scalar( 0, 255, 0 ), 2);
        /* distance = focalLength*11.0/bounding_rect.width; */
        focalLength = (bounding_rect.height* 24.0)/8.50;
        /* cout<<focalLength<<endl; */
        std::string title = std::to_string(focalLength) + "(in)";
        putText(frame, title, Point(50, 50),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0));
        largest_contour_index = 0;
        largest_area = 0;
        imshow( "result", frame);
        waitKey(1);
    } 
}
