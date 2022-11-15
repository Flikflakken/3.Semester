#ifndef PICTURE_HANDLING_H
#define PICTURE_HANDLING_H
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

class picture_handling
{
public:
    picture_handling();

    cv::Point3f findObject(cv::Mat input);

    cv::Matx33d homography();
};

#endif // PICTURE_HANDLING_H
