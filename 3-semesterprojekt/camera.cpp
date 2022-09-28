#include "camera.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pylon/PylonIncludes.h>

using namespace std;
using namespace cv;

camera::camera()
{

}

void camera::openWebcam()
{
    Mat camera;

    namedWindow("Display window");
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cout << "cannot open camera" << endl;
    }
    while (true){
        cap >> camera;
        imshow("Display window", camera);
        char c= (char)
                waitKey(20);
        if (c == 27) {
            break;
        }
    }

}

void camera::pylonTest(){
    int myExposure = 30000;

    // The exit code of the sample application.
    int exitCode = 0;

    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
    // is initialized during the lifetime of this object.
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Create an instant camera object with the camera device found first.
    Pylon::CInstantCamera camera( Pylon::CTlFactory::GetInstance().CreateFirstDevice());

    // Get a camera nodemap in order to access camera parameters.
    GenApi::INodeMap& nodemap= camera.GetNodeMap();

    // Open the camera before accessing any parameters.
    camera.Open();
}

