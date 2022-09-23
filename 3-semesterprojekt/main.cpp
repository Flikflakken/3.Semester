#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//-----Camera funktion---
//åbner webcam fra din maskine. HVIS det er en VM og Linux, skal der installeres "Cheese"(Dette er webcam app til linux.)
//og der skal hentes pakker til dit VM software, for at tillade brugen af webcam. VirtualBox: https://www.virtualbox.org/wiki/Downloads
int camera(){
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
    return 0;
}

int main()
{
    cout << "Hello World!" << endl;
    camera();
    return 0;
}
