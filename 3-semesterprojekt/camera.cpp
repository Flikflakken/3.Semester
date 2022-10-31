#include "camera.h"
#include <iostream>
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>

using namespace std;
using namespace cv;
using namespace Pylon;


camera::camera()
{

}

void camera::calibrateCamera()
{
    cv::Matx33f K(cv::Matx33f::eye());  // intrinsic camera matrix
    cv::Vec<float, 5> k(0, 0, 0, 0, 0); // distortion coefficients

    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<double> stdIntrinsics, stdExtrinsics, perViewErrors;
    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 +
            cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT;
    cv::Size frameSize(1440, 1080);


    std::vector<cv::String> fileNames;
    cv::glob("/home/mikkel/programmering/semesterprojekt/Pic_cali/Image*.bmp", fileNames, false);
    cv::Size patternSize(10 - 1, 7 - 1);
    std::vector<std::vector<cv::Point2f>> q(fileNames.size());

    std::vector<std::vector<cv::Point3f>> Q;
    // 1. Generate checkerboard (world) coordinates Q. The board has 10 x 7
    // fields with a size of 50x50mm

    int checkerBoard[2] = {10,7};
    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for(int i = 1; i<checkerBoard[1]; i++){
        for(int j = 1; j<checkerBoard[0]; j++){
            objp.push_back(cv::Point3f(j,i,0));
        }
    }

    std::vector<cv::Point2f> imgPoint;
    // Detect feature points
    std::size_t i = 0;
    for (auto const &f : fileNames) {

        // 2. Read in the image an call cv::findChessboardCorners()
        cv::Mat img = cv::imread(fileNames[i]);
        cv::Mat gray;

        cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);

        bool patternFound = cv::findChessboardCorners(gray, patternSize, q[i], cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        // 2. Use cv::cornerSubPix() to refine the found corner detections
        if(patternFound){
            cv::cornerSubPix(gray, q[i],cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
            Q.push_back(objp);
        }
        i++;
    }

    std::cout << "Calibrating..." << std::endl;
    // 4. Call "float error = cv::calibrateCamera()" with the input coordinates
    // and output parameters as declared above...

    float error = cv::calibrateCamera(Q, q, frameSize, K, k, rvecs, tvecs, flags);

    std::cout << "Reprojection error = " << error << "\nK =\n"
              << K << "\nk=\n"
              << k << std::endl;

}

Mat camera::correction(const cv::Mat& src)
{
    cv::Matx33f K(1238.3773, 0, 719.5,
                  0, 1238.3773, 539.5,
                  0, 0, 1);  // intrinsic camera matrix

    cv::Vec<float, 5> k(-0.244835, 0.129333, 0, 0, 0); // distortion coefficients

    cv::Size frameSize(1440,1080);
    cout << src.rows << endl;
    cout << src.cols << endl;
    cout << src.type() << endl;

    Mat output, mapX, mapY;
    cv::initUndistortRectifyMap(K, k, cv::Matx33f::eye(),
                                K, frameSize, CV_32FC1, mapX, mapY);

    cv::remap(src, output, mapX, mapY, cv::INTER_LINEAR);
    cout << output.rows << endl;
    cout << output.cols << endl;

    cv::imshow("after Remap", output);
    cv::waitKey(0);

    return output;
}

void camera::correction_test() {

    Mat src = imread("/home/mikkel/Desktop/Image__2022-10-25__11-41-53.bmp");
    namedWindow( "src", cv::WINDOW_AUTOSIZE);
    imshow("src", src);
    waitKey(0);

    cv::Matx33f K(1238.3773, 0, 719.5,
                  0, 1238.3773, 539.5,
                  0, 0, 1);  // intrinsic camera matrix

    cv::Vec<float, 5> k(-0.244835, 0.129333, 0, 0, 0); // distortion coefficients

    cv::Size frameSize(1440,1080);
    cout << src.rows << endl;
    cout << src.cols << endl;
    cout << src.type() << endl;

    Mat output(src.size(), src.type());
    Mat mapX(src.size(), CV_16SC2);
    Mat mapY(src.size(), CV_16SC2);

    cv::initUndistortRectifyMap(K, k, cv::Matx33f::eye(),
                                K, frameSize, CV_32FC1, mapX, mapY);

    cv::remap(src, output, mapX, mapY, cv::INTER_LINEAR);
    cout << output.rows << endl;
    cout << output.cols << endl;

    cv::imshow("after Remap", output);
    cv::waitKey(0);

}

cv::Mat camera::grabPic(){

    PylonAutoInitTerm autoInitTerm;  // PylonInitialize() will be called now
    // Create an instant camera object with the camera device found first.
    CBaslerUniversalInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice() );
    camera.Open();
    camera.Width.SetValue( 1440 );
    camera.Height.SetValue( 1080 );


    // Print the model name of the camera.
    cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
    // Create a pylon ImageFormatConverter object.
    CImageFormatConverter formatConverter;
    // Create a PylonImage that will be used to create OpenCV images later.
    CPylonImage pylonImage;
    // Create an OpenCV image.
    Mat openCvImage;
    // This smart pointer will receive the grab result data.
    CGrabResultPtr ptrGrabResult;

    // Specify the output pixel format.
    formatConverter.OutputPixelFormat = PixelType_BGR8packed;

    //camera.GrabOne(5000, ptrGrabResult, TimeoutHandling_ThrowException);
    //grab one image
    camera.StartGrabbing( 1, GrabStrategy_OneByOne, GrabLoop_ProvidedByUser);

    //grab is stopped automatically due to maxImages = 1
    camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException) && ptrGrabResult->GrabSucceeded();

    // Convert the grabbed buffer to a pylon image.
    formatConverter.Convert(pylonImage, ptrGrabResult);

    // Create an OpenCV image from a pylon image.
    openCvImage = Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

    // Create an OpenCV display window.
    namedWindow( "myWindow", cv::WINDOW_AUTOSIZE); // other options: CV_AUTOSIZE, CV_FREERATIO
    // Display the current image in the OpenCV display window.
    imshow( "myWindow", openCvImage);
    waitKey(0);
    return openCvImage;
}

string camera::MatType2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}


