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

void picture_handling::findObject()
{
    Mat input = imread("/home/mikkel/Desktop/Image__2022-10-25__11-41-53.bmp");
//    imshow("Window", input);
//    waitKey(0);

    Mat gray;
    cvtColor(input, gray, COLOR_BGR2GRAY);

    medianBlur(gray, gray, 5);

    Mat dst(gray.size(), CV_16UC1);
    threshold(gray, dst, 0, 255, THRESH_OTSU);
//    imshow("Window", dst);
//    waitKey(0);

    //Mathematical Morphology -----------------------------------------
    int morph_size = 3;
    Mat morphEle = getStructuringElement(MORPH_CROSS, Size(3, 3));

    Mat erodsion;
    erode(dst, erodsion, morphEle, Point(-1,-1),6);
//    imshow("After Erosion", erodsion);
//    waitKey(0);

    Mat dilation;
    Mat dilatEle = getStructuringElement(MORPH_CROSS, Size(3,3));
    dilate(erodsion, dilation, dilatEle,Point(-1,-1),6);
//    imshow("After Diliation", dilation);
//    waitKey(0);
    //-----------------------------------------------------------------

    std::vector<Vec3f> circles;
    HoughCircles(dilation, circles, HOUGH_GRADIENT, 2, 20, 300, 20, 27, 28);

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(input, center, 1, Scalar(0,100,100), 3, LINE_AA);
        cout << "center point : " << center << endl;
        // circle outline
        int radius = c[2];
        circle(input, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }
    imshow("detected circles", input);
    waitKey(0);
}

void picture_handling::homography()
{
    // Læser input image.
    Mat input_img = imread("/home/mikkel/Desktop/Image__2022-10-07__16-55-22.bmp");
    imshow("input", input_img);
    waitKey(0);

    Matx33f Camera_Matrix(1238.3773, 0.0, 719.5,
                          0.0, 1238.3773, 539.5,
                          0.0, 0.0, 1.0);
    Vec<float, 5> DistCoeff(-0.244835, 0.1293330, 0, 0, 0);

    Mat ImageUndistorted;
    undistort(input_img, ImageUndistorted, Camera_Matrix, DistCoeff);
    imshow("Undistort", ImageUndistorted);
    waitKey(0);

    //HOMOGRAPHY MATRIX
    // 4 Valgte punkter i billede
    vector<Point2f> Hpoints_image;
    Hpoints_image.push_back(Point2f(610, 613));
    Hpoints_image.push_back(Point2f(865, 613));
    Hpoints_image.push_back(Point2f(610, 781));
    Hpoints_image.push_back(Point2f(865, 781));

    // 4 Punkter i RW i mm
    vector<Point2f> Hpoints_RW;
    Hpoints_RW.push_back(Point2f(250, 250));
    Hpoints_RW.push_back(Point2f(550, 250));
    Hpoints_RW.push_back(Point2f(250, 450));
    Hpoints_RW.push_back(Point2f(550, 450));


    // Udregn Homography
    Mat Homography = findHomography(Hpoints_image, Hpoints_RW);
    cout << "Homography = \n" << Homography << "\n" << endl;


}


