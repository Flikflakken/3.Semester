#include "picture_handling.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

picture_handling::picture_handling()
{

}

Point3f picture_handling::findObject(Mat input)
{
    Mat gray;
    cvtColor(input, gray, COLOR_BGR2GRAY);

    medianBlur(gray, gray, 5);

    Mat dst(gray.size(), CV_16UC1);
    threshold(gray, dst, 0, 255, THRESH_OTSU);
    //Mathematical Morphology -----------------------------------------
    int morph_size = 3;
    Mat morphEle = getStructuringElement(MORPH_CROSS, Size(3, 3));

    Mat erodsion;
    erode(dst, erodsion, morphEle, Point(-1,-1),6);

    Mat dilation;
    Mat dilatEle = getStructuringElement(MORPH_CROSS, Size(3,3));
    dilate(erodsion, dilation, dilatEle,Point(-1,-1),6);
    //-----------------------------------------------------------------

    std::vector<Vec3f> circles;
    HoughCircles(dilation, circles, HOUGH_GRADIENT, 2, 20, 300, 20, 27, 28);

    Matx33f Homography_Matrix(1.176470588235294, 0, -467.6470588235295,
                              0, 1.190476190476191, -479.7619047619052,
                              0, 0, 1);
    Point3f pixelPoint;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        // circle center
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        pixelPoint.x = center.y;
        pixelPoint.y = center.x;
        pixelPoint.z = 1;
        circle(input, center, 1, Scalar(0,100,100), 3, LINE_AA);
        cout << "center point : " << center << endl;
        // circle outline
        int radius = c[2];
        circle(input, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }

    //imshow("detected circles", input);
    //waitKey(0);
    cout << pixelPoint << endl;
    return pixelPoint;
}

cv::Matx33d picture_handling::homography()
{
    Matx33f Camera_Matrix(1238.3773, 0.0, 719.5,
                          0.0, 1238.3773, 539.5,
                          0.0, 0.0, 1.0);
    Vec<float, 5> DistCoeff(-0.244835, 0.1293330, 0, 0, 0);

    /*
    Mat ImageUndistorted;
    undistort(input, ImageUndistorted, Camera_Matrix, DistCoeff);
    imshow("Undistort", ImageUndistorted);
    waitKey(0);
    */
    //HOMOGRAPHY MATRIX
    // 4 Valgte punkter i billede
    vector<Point2d> Hpoints_image;
    Hpoints_image.push_back(Point2d(611, 613));
    Hpoints_image.push_back(Point2d(865, 613));
    Hpoints_image.push_back(Point2d(610, 781));
    Hpoints_image.push_back(Point2d(865, 781));
    /*
    Hpoints_image.push_back(Point2f(440, 445));
    Hpoints_image.push_back(Point2f(695, 740));
    Hpoints_image.push_back(Point2f(908, 568));
    Hpoints_image.push_back(Point2f(568, 869)); //
    Hpoints_image.push_back(Point2f(992, 654)); //654
    Hpoints_image.push_back(Point2f(994, 826));
    */
    // 4 Punkter i RW i mm
    vector<Point2d> Hpoints_RW;
    Hpoints_RW.push_back(Point2d(250, 250));
    Hpoints_RW.push_back(Point2d(550, 250));
    Hpoints_RW.push_back(Point2d(250, 450));
    Hpoints_RW.push_back(Point2d(550, 450));
    /*
    Hpoints_RW.push_back(Point2f(50,50));
    Hpoints_RW.push_back(Point2f(400, 400));
    Hpoints_RW.push_back(Point2f(200, 600));
    Hpoints_RW.push_back(Point2f(550, 200));
    Hpoints_RW.push_back(Point2f(300, 700));
    Hpoints_RW.push_back(Point2f(500,700));
    */
    // Udregn Homography
    Matx33d Homography = findHomography(Hpoints_image, Hpoints_RW);
    cout << "Homography = \n" << Homography << "\n" << endl;

    return Homography;
}


