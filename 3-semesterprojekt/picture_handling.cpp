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

Point3f picture_handling::findObject(Mat input, int minRadius, int maxRadius)
{
    Mat gray;
    cvtColor(input, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 5);
    Mat dst(gray.size(), CV_16UC1);
    threshold(gray, dst, 0, 255, THRESH_OTSU);
    //imshow("Threshold", dst);
    //waitKey(0);
    //Mathematical Morphology -----------------------------------------
    int morph_size = 3;
    Mat morphEle = getStructuringElement(MORPH_CROSS, Size(3, 3));
    Mat erodsion;
    erode(dst, erodsion, morphEle, Point(-1,-1),7);
    //imshow("derode", erodsion);
    //waitKey(0);
    Mat dilation;
    Mat dilatEle = getStructuringElement(MORPH_CROSS, Size(3,3));
    dilate(erodsion, dilation, dilatEle,Point(-1,-1),7);
    //imshow("dialate", dilation);
    //waitKey(0);
    //-----------------------------------------------------------------
    std::vector<Vec3f> circles;
    HoughCircles(dilation, circles, HOUGH_GRADIENT, 2, 20, 30, 20, minRadius, maxRadius);

    Point3f pixelPoint;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        // circle center
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        pixelPoint.x = center.x;
        pixelPoint.y = center.y;
        pixelPoint.z = 1;
        circle(input, center, 1, Scalar(0,100,100), 3, LINE_AA);
        cout << "center point : " << center << endl;
        // circle outline
        int radius = c[2];
        circle(input, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }

    namedWindow("detected circles", WINDOW_FREERATIO);
    imshow("detected circles", input);
    waitKey(0);
    cout << "PixelPoint" << pixelPoint << endl;
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


cv::Matx33d picture_handling::homography2() {
    Matx33f Camera_Matrix(1238.3773, 0.0, 719.5,
                          0.0, 1238.3773, 539.5,
                          0.0, 0.0, 1.0);
    Vec<float, 5> DistCoeff(-0.244835, 0.1293330, 0, 0, 0);

    //HOMOGRAPHY MATRIX
    // 4 Valgte punkter i billede
    vector<Point2d> Hpoints_image;
    Hpoints_image.push_back(Point2d(482, 486));
    Hpoints_image.push_back(Point2d(556, 487));
    Hpoints_image.push_back(Point2d(651, 485));
    Hpoints_image.push_back(Point2d(736, 484));
    Hpoints_image.push_back(Point2d(820, 484));
    Hpoints_image.push_back(Point2d(905, 484));
    Hpoints_image.push_back(Point2d(991, 484));
    Hpoints_image.push_back(Point2d(482, 655));
    Hpoints_image.push_back(Point2d(568, 656));
    Hpoints_image.push_back(Point2d(652, 655));
    Hpoints_image.push_back(Point2d(737, 653));
    Hpoints_image.push_back(Point2d(820, 654));
    Hpoints_image.push_back(Point2d(907, 654));
    Hpoints_image.push_back(Point2d(997, 654));
    Hpoints_image.push_back(Point2d(483, 827));
    Hpoints_image.push_back(Point2d(568, 827));
    Hpoints_image.push_back(Point2d(553, 827));
    Hpoints_image.push_back(Point2d(738, 826));
    Hpoints_image.push_back(Point2d(824, 825));
    Hpoints_image.push_back(Point2d(908, 826));
    //Hpoints_image.push_back(Point2d(996, 824));

    // 4 Punkter i RealWorld i mm
    vector<Point2d> Hpoints_RW;
    Hpoints_RW.push_back(Point2d(100, 100));
    Hpoints_RW.push_back(Point2d(100, 200));
    Hpoints_RW.push_back(Point2d(100, 300));
    Hpoints_RW.push_back(Point2d(100, 400));
    Hpoints_RW.push_back(Point2d(100, 500));
    Hpoints_RW.push_back(Point2d(100, 600));
    Hpoints_RW.push_back(Point2d(100, 700));
    Hpoints_RW.push_back(Point2d(300, 100));
    Hpoints_RW.push_back(Point2d(300, 200));
    Hpoints_RW.push_back(Point2d(300, 300));
    Hpoints_RW.push_back(Point2d(300, 400));
    Hpoints_RW.push_back(Point2d(300, 500));
    Hpoints_RW.push_back(Point2d(300, 600));
    Hpoints_RW.push_back(Point2d(300, 700));
    Hpoints_RW.push_back(Point2d(500, 100));
    Hpoints_RW.push_back(Point2d(500, 200));
    Hpoints_RW.push_back(Point2d(500, 300));
    Hpoints_RW.push_back(Point2d(500, 400));
    Hpoints_RW.push_back(Point2d(500, 500));
    Hpoints_RW.push_back(Point2d(500, 600));
    //Hpoints_RW.push_back(Point2d(500, 700));

    // Udregn Homography
    Matx33d Homography = findHomography(Hpoints_image, Hpoints_RW);
    cout << "Homography = \n" << Homography << "\n" << endl;


    return Homography;
}


