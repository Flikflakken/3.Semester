#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pylon/PylonIncludes.h>
#include "camera.h"
#include "camera_cali.h"
#include "picture_handling.h"


using namespace std;
using namespace cv;
using namespace Pylon;

//-----Camera funktion---
//åbner webcam fra din maskine. HVIS det er en VM og Linux, skal der installeres "Cheese"(Dette er webcam app til linux.)
//og der skal hentes pakker til dit VM software, for at tillade brugen af webcam. VirtualBox: https://www.virtualbox.org/wiki/Downloads


int main()
{
    //Mat pic;
    //camera().correction_test();
    //pic = camera().grabPic();

    Mat src = imread("/home/mikkel/Desktop/Image__2022-10-25__11-41-53.bmp");
    picture_handling().homography();
    picture_handling().findObject();
/*
    string ty =  camera().MatType2str(pic.type());
    printf(" Pic Matrix: %s %dx%d \n", ty.c_str(), pic.cols, pic.rows );

    string ty2 =  camera().MatType2str(src.type());
    printf("Src Matrix: %s %dx%d \n", ty.c_str(), pic.cols, pic.rows );
*/

//    Mat pic(1440,1080, CV_8UC1);
//    Mat pic_afterCali(1440,1080, CV_8UC1);
//    pic = camera().pylonTest();
//    pic_afterCali = camera_cali().calibrate_camera(pic);





//    namedWindow( "AfterCali", WINDOW_FREERATIO); // other options: CV_AUTOSIZE, CV_FREERATIO
//    imshow( "AfterCali", pic_afterCali);
//    waitKey(0);

//    int myExposure = 30000;

//    // The exit code of the sample application.
//    int exitCode = 0;

//    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
//    // is initialized during the lifetime of this object.
//    Pylon::PylonAutoInitTerm autoInitTerm;

//    try
//    {
//        // Create an instant camera object with the camera device found first.
//        Pylon::CInstantCamera camera( Pylon::CTlFactory::GetInstance().CreateFirstDevice());

//        // Get a camera nodemap in order to access camera parameters.
//        GenApi::INodeMap& nodemap= camera.GetNodeMap();

//        // Open the camera before accessing any parameters.
//        camera.Open();
//        // Create pointers to access the camera Width and Height parameters.
//        GenApi::CIntegerPtr width= nodemap.GetNode("Width");
//        GenApi::CIntegerPtr height= nodemap.GetNode("Height");

//        // The parameter MaxNumBuffer can be used to control the count of buffers
//        // allocated for grabbing. The default value of this parameter is 10.
//        //camera.MaxNumBuffer = 5;

//        // Create a pylon ImageFormatConverter object.
//        Pylon::CImageFormatConverter formatConverter;
//        // Specify the output pixel format.
//        formatConverter.OutputPixelFormat= Pylon::PixelType_BGR8packed;
//        // Create a PylonImage that will be used to create OpenCV images later.
//        Pylon::CPylonImage pylonImage;

//        // Create an OpenCV image.
//        cv::Mat openCvImage;


//        // Set exposure to manual
//        GenApi::CEnumerationPtr exposureAuto( nodemap.GetNode( "ExposureAuto"));
//        if ( GenApi::IsWritable( exposureAuto)){
//            exposureAuto->FromString("Off");
//            std::cout << "Exposure auto disabled." << std::endl;
//        }

//        // Set custom exposure
//        GenApi::CFloatPtr exposureTime = nodemap.GetNode("ExposureTime");
//        std::cout << "Old exposure: " << exposureTime->GetValue() << std::endl;
//        if(exposureTime.IsValid()) {
//            if(myExposure >= exposureTime->GetMin() && myExposure <= exposureTime->GetMax()) {
//                exposureTime->SetValue(myExposure);
//            }else {
//                exposureTime->SetValue(exposureTime->GetMin());
//                std::cout << ">> Exposure has been set with the minimum available value." << std::endl;
//                std::cout << ">> The available exposure range is [" << exposureTime->GetMin() << " - " << exposureTime->GetMax() << "] (us)" << std::endl;
//            }
//        }else {

//            std::cout << ">> Failed to set exposure value." << std::endl;
//            return false;
//        }
//        std::cout << "New exposure: " << exposureTime->GetValue() << std::endl;

//        // Start the grabbing of c_countOfImagesToGrab images.
//        // The camera device is parameterized with a default configuration which
//        // sets up free-running continuous acquisition.
//        camera.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);

//        // This smart pointer will receive the grab result data.
//        Pylon::CGrabResultPtr ptrGrabResult;

//        // image grabbing loop
//        int frame = 1;
//        Mat afterOTSU;
//        while ( camera.IsGrabbing())
//        {
//            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
//            camera.RetrieveResult( 5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

//            // Image grabbed successfully?
//            if (ptrGrabResult->GrabSucceeded())
//            {
//                // Access the image data.
//                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
//                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;

//                // Convert the grabbed buffer to a pylon image.
//                formatConverter.Convert(pylonImage, ptrGrabResult);

//                // Create an OpenCV image from a pylon image.
//                openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t *) pylonImage.GetBuffer());

//                //////////////////////////////////////////////////////
//                //////////// Here your code begins ///////////////////
//                //////////////////////////////////////////////////////

//                // Create an OpenCV display window.
//                cv::namedWindow( "myWindow", cv::WINDOW_FREERATIO); // other options: CV_AUTOSIZE, CV_FREERATIO

//                // Display the current image in the OpenCV display window.
//                cv::imshow( "myWindow", openCvImage);


//                // Detect key press and quit if 'q' is pressed
//                int keyPressed = cv::waitKey(1);
//                if(keyPressed == 'q'){ //quit
//                    std::cout << "Shutting down camera..." << std::endl;
//                    camera.Close();
//                    std::cout << "Camera successfully closed." << std::endl;
//                    break;
//                }

//                ////////////////////////////////////////////////////
//                //////////// Here your code ends ///////////////////
//                ////////////////////////////////////////////////////




//                frame++;

//            }
//            else
//            {
//                std::cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << std::endl;
//            }
//        }

//    }
//    catch (GenICam::GenericException &e)
//    {
//        // Error handling.
//        std::cerr << "An exception occurred." << std::endl
//        << e.GetDescription() << std::endl;
//        exitCode = 1;
//    }

//    return exitCode;
}
