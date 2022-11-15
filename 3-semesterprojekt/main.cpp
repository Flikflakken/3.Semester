#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pylon/PylonIncludes.h>
#include "camera.h"
#include "camera_cali.h"
#include "picture_handling.h"
#include "gripper.h"
#include "tcp.h"


using namespace std;
using namespace cv;
using namespace Pylon;

int main()
{
    /*
    tcp().tcpSocket_Connect();
    tcp().doMove(50);
    tcp().sayBye();

    //Test kode til Daniel's TCP
    Gripper gripper{};
    gripper.doInit("127.0.0.1", 1000, 30000, 30000);
    gripper.doConnect();
    gripper.doHome(1);
    gripper.doMove(50);
    gripper.sayBye();
     */

    //TEST AF PIC OG BOLD PLACERING
    Mat pic;
    Matx33d homography;
    Point3d pixelPoints;
    pic = camera().grabPic();
    //camera().correction_test();
    pic = camera().correction(pic);
    homography = picture_handling().homography();
    pixelPoints = picture_handling().findObject(pic);
    Point3d tablePoint = homography * pixelPoints;
    Affine3<double> robotRotation(-0.3841, -0.9233, 0,
                                  0.9233, -0.3841, 0,
                                  0, 0, 1.0000);
    Point3d robotTranslation(702.1507, -709.9849, -18.0000);
    Point3f robotPoint = robotRotation * tablePoint + robotTranslation;
    //Nu skal vi have tablePoint til robotPuint. Derfor:
    //Rotation*tablePoint+Translation.

    cout << "Main: " << homography << endl;
    cout << tablePoint << endl;



}
