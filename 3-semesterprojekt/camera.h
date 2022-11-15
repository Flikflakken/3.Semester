#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>

class camera
{
public:
    camera();
    void calibrateCamera();
    cv::Mat correction(const cv::Mat& input);
    void correction_test();
    cv::Mat grabPic();
    std::string MatType2str(int type);
};

#endif // CAMERA_H
