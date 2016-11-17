#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <QApplication>
#include <iostream>
#include <thread>

//void processSLAM(PUTSLAM* slam){
//    slam->process();
//}
// test

void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D){
    while(1){
        Mat34 camPose;
        //slam.getPose(camPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        //slam.getFrame(rgbImg, depthImg);
        cv::Mat mask;
        objLoader->computeMask(camPose, mask);

        visu2D->updateMask(mask);
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
        std::string visualizerConfig(config.FirstChildElement( "Visualizer" )->FirstChildElement("config")->GetText());
        std::string visualizerType(config.FirstChildElement( "Visualizer" )->FirstChildElement("type")->GetText());

        QApplication application(argc,argv);
        setlocale(LC_NUMERIC,"C");
        glutInit(&argc, argv);

        QGLVisualizer visu(visualizerConfig);
        visu.setWindowTitle("Simulator viewer");
        visu.show();

        ObjLoader* objLoader = putar::createMyLoader();
        objLoader->attachVisualizer(&visu);

        objLoader->loadObj("kamien.obj");

        //PUTSLAM slam;

        ImageVisualizer* visu2D = putar::createMyImageVisualizer();

        Hmi* hmiDev = putar::createMyHmiGamepad();

        //std::thread processThr(processSLAM, &slam);

        std::thread putarThr(processPUTAR, objLoader, visu2D);

        application.exec();

        //processThr.join();
        putarThr.join();

        std::cout << "Done\n";
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
