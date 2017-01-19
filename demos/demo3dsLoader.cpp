//#include <GL/glew.h>
#include "Defs/defs.h"
#include "ObjLoader/objLoader.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <QApplication>
#include <iostream>
#include <thread>



/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/





//Now the object is generic, the cube has annoyed us a little bit, or not?
obj_type object;

#define JOY_DEV "/dev/input/js0"
#define GLUT_JOYSTICK_BUTTON_E 128
#define GLUT_JOYSTICK_BUTTON_F 512
#define GLUT_JOYSTICK_BUTTON_AB 18
#define GLUT_JOYSTICK_BUTTON_BC 3
#define GLUT_JOYSTICK_BUTTON_AD 24
#define GLUT_JOYSTICK_BUTTON_CD 9
#define GLUT_JOYSTICK_BUTTON_G 16


void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D){
    while(1){
        Mat34 camPose;
        //slam.getPose(camPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        //slam.getFrame(rgbImg, depthImg);
        cv::Mat mask;
        //DB objLoader->computeMask(camPose, mask);

        //visu2D->updateMask(mask, mask);
        //visu2D->updateFrame(rgbImg,depthImg);
    }

}

int main(int argc, char** argv)
{
    try {
        using namespace putar;
        using namespace std;

        tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";
        std::string visualizerConfig(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("config")->GetText());
        std::string visualizerType(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("type")->GetText());
        std::string Loader3dsConfig(config.FirstChildElement("Configuration")->FirstChildElement("Loader3ds")->FirstChildElement("config")->GetText());

        QApplication application(argc,argv);
        setlocale(LC_NUMERIC,"C");
        glutInit(&argc, argv);

        QGLVisualizer visu(visualizerConfig);
        visu.resize(640, 480);
        visu.setWindowTitle("Simulator viewer");
        visu.show();


        ObjLoader* objLoader;
        if (0)
            objLoader = putar::createMyLoader();
        else{
            objLoader = putar::createMy3dsLoader(Loader3dsConfig);
        }
        objLoader->attachVisualizer(&visu);

        putar::obj_type object;

        objLoader->loadObj();

        objLoader->getMesh(object);

///////////////////////////////////////////////////////////////////////////////////////////////////////

        cv::Mat rgbMask;
        Mat34 cameraPose;
        Mat34 objectPose;
        cv::Mat depthMask;
        Hmi* hmiDev = putar::createMyHmiGamepad("HmiGamepadConfig.xml");



        while(1)
        {
            hmiDev->updatePose(objectPose);
            std::cout<<objectPose.matrix()<<std::endl;
            objLoader->computeMask(cameraPose,objectPose, rgbMask, depthMask);

            cv::namedWindow("imgMAT");
            cv::imshow("imgMAT", depthMask);
            cv::namedWindow("imgMAT2");
            cv::imshow("imgMAT2", rgbMask);
            cv::waitKey(300);

        }


        cv::waitKey(0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "Done\n";
}
catch (const std::exception& ex) {
std::cerr << ex.what() << std::endl;
return 1;
}

return 0;
}
