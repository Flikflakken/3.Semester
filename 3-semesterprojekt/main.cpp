#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pylon/PylonIncludes.h>
#include "camera.h"
#include "camera_cali.h"
#include "picture_handling.h"
#include "gripper.h"
#include "tcp.h"
#include <boost/thread/thread.hpp>
#include <ur_rtde/rtde_io_interface.h>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>

#include "thread"
#include "chrono"
#include "unistd.h"


using namespace std;
using namespace cv;
using namespace Pylon;
using namespace ur_rtde;

int main()
{
    //SETUP-------------------
    tcp tcpgripper{};
    tcpgripper.setMTimeOut(3000);
    tcpgripper.setMip("192.168.100.10");
    tcpgripper.setMPort(1000);
    tcpgripper.connectTcp();
    tcpgripper.verbose(1);
    tcpgripper.home();
    RTDEControlInterface rtde_control("192.168.100.11");
    RTDEReceiveInterface rtde_recive("192.168.100.11");
    std::vector<double> homePose = {0.094, -0.421, 0.25, -0.598, 3.075, -0.047};

    //TEST AF PIC OG BOLD PLACERING
    rtde_control.moveL(homePose);
    Mat pic;
    Matx33d homography;
    Point3d pixelPoints;
    pic = camera().grabPic();
    //camera().correction_test();
    pic = camera().correction(pic);
    homography = picture_handling().homography();
    pixelPoints = picture_handling().findObject(pic);
    Point3d tablePoint = homography * pixelPoints;
    //Robot Rotationsmatrix fra kalibering i MatLab
    Matx33d robotRotation(-0.3841, -0.9233, 0,
                                  0.9233, -0.3841, 0,
                                  0, 0, 1.0000);
    //Robot Translation fra kalibering i MatLabhomePoseRotation
    Point3d robotTranslation(702.1507, -709.9849, -18.0000);
    //Nu skal vi have tablePoint til robotPuint. Derfor:
    //Rotation*tablePoint+Translation.
    Point3d robotPoint = robotRotation * tablePoint + robotTranslation;
    std::vector<double> objectXYZPose;
    cout << tablePoint << endl;
    cout << robotPoint << endl;

    //divider med 1000, da robot måler i meter.
    objectXYZPose.push_back(robotPoint.x/1000);
    objectXYZPose.push_back(robotPoint.y/1000);
    objectXYZPose.push_back(0.175);
    objectXYZPose.push_back(-0.598);
    objectXYZPose.push_back(3.075);
    objectXYZPose.push_back(-0.047);

    for (int i = 0; i < objectXYZPose.size(); ++i) {
        cout << objectXYZPose[i] << endl;
    }

    rtde_control.moveL(objectXYZPose,0.3,2);
    tcpgripper.grip(30);
    sleep(2);
    rtde_control.moveL(homePose);
    tcpgripper.home();

}
