#ifndef CAMERA_CALI_H
#define CAMERA_CALI_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>

class camera_cali
{
public:
    camera_cali();
    cv::Mat calibrate_camera(cv::Mat input);
};

#endif // CAMERA_CALI_H
