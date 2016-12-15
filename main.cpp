#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <QApplication>
#include <iostream>
#include <thread>
#include <PUTSLAM/PUTSLAM.h>

PUTSLAM slam;

void processSLAM(PUTSLAM* _slam){
    _slam->startProcessing();
}
 //test





void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D, Hmi* hmiDev){
    while(1){
        Mat34 camPose;
        slam.getCurrentPose(camPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        slam.getCurrentFrame(rgbImg, depthImg);
        cv::Mat mask;
        Mat34 objPose;
        hmiDev->updatePose(objPose);
        objLoader->computeMask(camPose, mask);

        visu2D->updateMask(mask, mask);
        visu2D->updateFrame(rgbImg,depthImg);
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
        visu.setWindowTitle("Simulator viewer");
        visu.show();

        ObjLoader* objLoader;// = putar::createMyLoader();
        if (0)
            objLoader = putar::createMyLoader();
        else{
            objLoader = putar::createMy3dsLoader(Loader3dsConfig);
        }
        objLoader->attachVisualizer(&visu);

        //objLoader->loadObj("kamien.obj");



        ImageVisualizer* visu2D = putar::createMyImageVisualizer("ImageVisualizerConfig.xml");


        Hmi* hmiDev = putar::createMyHmiGamepad("HmiGamepadConfig.xml");

        std::thread processThr(processSLAM, &slam);

        std::thread putarThr(processPUTAR, objLoader, visu2D, hmiDev);

        application.exec();

        processThr.join();
        putarThr.join();

        std::cout << "Done\n";
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
