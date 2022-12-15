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

    // ------------PRIMÆR KODE-----------------------------------
    //SETUP-------------------
    tcp tcpgripper{};
    tcp tcpMatlab{};
    tcpgripper.setMTimeOut(3000);
    tcpgripper.setMip("192.168.100.10");
    tcpgripper.setMPort(1000);
    tcpgripper.connectTcp();
    tcpgripper.verbose(1);
    tcpgripper.home();
    //sleep(2);
    RTDEControlInterface rtde_control("192.168.100.11");
    RTDEReceiveInterface rtde_recive("192.168.100.11");
    std::vector<double> homePose = {0.094, -0.421, 0.25, -0.598, 3.075, -0.047};

    //PIC OG BOLD PLACERING
    rtde_control.moveL(homePose);
    Mat Pic;
    Matx33d homography;
    Point3d pixelPointsBall;
    Point3d pixelPointsTarget;
    Pic = camera().grabPic();
    Pic = camera().correction(Pic);
    homography = picture_handling().homography2();
    //Matx33d Homography_Matrix(1.198316650419649, 0.01307686820894459, -486.543902612996,
    //                                       0, 1.218526355833472, -493.3129651204262,
    //                                       0, 0, 1);
    pixelPointsBall = picture_handling().findObject(Pic, 17, 18);
    pixelPointsTarget = picture_handling().findObject(Pic, 52, 54);
    Point3d tablePointBall = homography * pixelPointsBall;
    Point3d tablePointTarget = homography * pixelPointsTarget;

    //Normaliser TablePoint
    tablePointBall = tablePointBall/tablePointBall.z;
    tablePointTarget = tablePointTarget/tablePointTarget.z;
    //Robot Rotationsmatrix fra kalibering i MatLab
    Matx33d robotRotation(-0.3841, -0.9233, 0,
                                  0.9233, -0.3841, 0,
                                  0, 0, 1.0000);
    //Robot Translation fra kalibering i MatLabhomePoseRotation
    Point3d robotTranslation(702.1507, -709.9849, -18.0000);
    //Nu skal vi have tablePoint til robotPuint. Derfor:
    Point3d robotPointBall = robotRotation * tablePointBall + robotTranslation;
    //Point3d robotPointTarget = robotRotation * tablePointBall + robotTranslation;
    cout << "TablePointBall" << tablePointBall << endl;
    cout << "robotPointBall" << robotPointBall << endl;
    cout << "TablePointTarget" << tablePointTarget << endl;

    std::vector<double> objectXYZPose;
    //divider med 1000, da robot måler i meter.
    objectXYZPose.push_back(robotPointBall.x/1000);
    objectXYZPose.push_back(robotPointBall.y/1000);
    objectXYZPose.push_back(0.173);
    objectXYZPose.push_back(-0.598);
    objectXYZPose.push_back(3.075);
    objectXYZPose.push_back(-0.047);

    std::vector<double> objectXYZPoseHigh;
    //divider med 1000, da robot måler i meter.
    objectXYZPoseHigh.push_back(robotPointBall.x/1000);
    objectXYZPoseHigh.push_back(robotPointBall.y/1000);
    objectXYZPoseHigh.push_back(0.300);
    objectXYZPoseHigh.push_back(-0.598);
    objectXYZPoseHigh.push_back(3.075);
    objectXYZPoseHigh.push_back(-0.047);


    for (int i = 0; i < objectXYZPose.size(); ++i) {
        cout << objectXYZPose[i] << endl;
    }

    //Gamle kaste kode:
    //std::vector<double> joint_speed = {0.3199, -2.4502, 3.1909, 0.0, 0.0, 0.0};

    rtde_control.moveL(objectXYZPoseHigh,0.3,2);
    rtde_control.moveL(objectXYZPose,0.3,2);

    tcpgripper.grip(10, 40);
    sleep(2);
    //std::this_thread::sleep_for(std::chrono::duration<double>(2000));
    /*
    rtde_control.moveJ(joint_start,0.3,2);
    rtde_control.speedJ(joint_speed, 10.6363, 0.3);
    std::this_thread::sleep_for(std::chrono::duration<double>(0.3));
    //sleep(0.25);
    tcpgripper.release();
    rtde_control.speedStop();
    rtde_control.moveL(homePose);
    */

    //-----------TEST TIL TARGET ------------------

    //Mat targetPic;
    //Point3d pixelPointsTarget;
    //targetPic = camera().grabPic();
    //targetPic = camera().correction(targetPic);
    //pixelPointsTarget = picture_handling().findObject(targetPic, 50, 55);
    //Point3d tablePointTarget = homography * pixelPointsTarget;

    string data;
    tcpMatlab.setMip("127.0.0.1");
    tcpMatlab.setMPort(1117);
    tcpMatlab.setMTimeOut(3000);
    tcpMatlab.connectTcpMatLab();
    cout << "Target x = " << tablePointTarget.x << endl;
    cout << "Target y = " << tablePointTarget.y << endl;

    data = tcpMatlab.sendDataMatLab((tablePointTarget.x-15), (tablePointTarget.y-15));
    cout << data << endl;

    vector<string> v;
    stringstream ss(data);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
    }
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }

    // 0 true / false
    // 1-6 er start pos.
    //7-12 hastighedsvector
    // 13 acc

    if(v[0] == "false"){
        cout << "Do not make this throw" << endl;
        return -1;
    }
    std::vector<double> ThrowStartPose;
    //divider med 1000, da robot måler i meter.
    ThrowStartPose.push_back(stod(v[1]));
    ThrowStartPose.push_back(stod(v[2]));
    ThrowStartPose.push_back(stod(v[3]));
    ThrowStartPose.push_back(stod(v[4]));
    ThrowStartPose.push_back(stod(v[5]));
    ThrowStartPose.push_back(stod(v[6]));

    std::vector<double> joint_start = {-1.1770, -0.7985, -2.0693, -0.6201, 1.4675, -0.0564};
    std::vector<double> joint_slip = {-1.12903, -1.16606, -1.59067, -0.620106, 1.46757, -0.056352};
    std::vector<double> joint_speed = {stod(v[7]), stod(v[8]), stod(v[9]), 0.0, 0.0, 0.0};
    double acceleration = stod(v[13]);
    rtde_control.moveJ(ThrowStartPose,0.3,2);
    rtde_control.speedJ(joint_speed, acceleration, 0.30);
    std::this_thread::sleep_for(std::chrono::duration<double>(0.30));
    //sleep(0.25);
    tcpgripper.release(15);
    rtde_control.speedStop();
    rtde_control.moveL(homePose);
}
