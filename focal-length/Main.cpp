#include "opencv2/imgproc.hpp"
using namespace cv; 
int main() {
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat frame, grey, blur, edged;
    VideoCapture cap(0);
    while(true){
        cap.read(frame);
        cvtColor(frame, grey, COLOR_BGR2GRAY);
        GaussianBlur(grey, blur, 5, 0, 0);
        Canny(blur, edged, 35, 125, 3);
        findContours(edged, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
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
        imshow( "result", frame);
        waitKey();
    } 
}