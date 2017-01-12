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
#include <putslam/PUTSLAM/PUTSLAM.h>

PUTSLAM slam;
cv::Mat rgbImg;
cv::Mat depthImg;

void processSLAM(PUTSLAM* _slam){
    _slam->startProcessing();
}
 //test





void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D, Hmi* hmiDev){
    usleep(1000000);
    while(1){
        Mat34 camPose;
        slam.getCurrentPose(camPose);
        slam.getCurrentFrame(rgbImg, depthImg);
        cv::imshow("putar rgb",rgbImg);
        cv::waitKey(30);
        Mat34 objPose;
        //hmiDev->updatePose(objPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        //slam.getFrame(rgbImg, depthImg);
Mat34 cameraPose;
        putar::obj_type object;
        cv::Mat rgbMask, depthMask;
        std::cout<<"--------------1"<<endl;
        objLoader->getMesh(object);
        std::cout<<"--------------2"<<endl;


        objLoader->computeMask(cameraPose, rgbMask, depthMask);
        std::cout<<"--------------3"<<endl;

        std::cout << "mask " << depthMask.rows << "\n";
        std::cout << "mask1 " << depthMask.cols << "\n";
//        cv::namedWindow("mask");
//        cv::imshow("mask",mask);
//        cv::waitKey(30);
//        cv::namedWindow("depthmask");
//        cv::imshow("depthmask",depthMask);
//        cv::waitKey(30);
        visu2D->updateMask(rgbMask, depthMask);
        visu2D->updateFrame(rgbImg,depthImg);

        break;
    }
}

int main(int argc, char** argv)
{
    try {   


        using namespace putar;
        using namespace std;
        std::cout<<"dziala"<<std::endl;

        tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";
        std::string visualizerConfig(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("config")->GetText());
        std::string visualizerType(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("type")->GetText());
        std::string Loader3dsConfig(config.FirstChildElement("Configuration")->FirstChildElement("Loader3ds")->FirstChildElement("config")->GetText());

//        QApplication application(argc,argv);
//        setlocale(LC_NUMERIC,"C");
        glutInit(&argc, argv);


//        QGLVisualizer visu(visualizerConfig);
//        visu.setWindowTitle("Simulator viewer");
//        visu.show();

        ObjLoader* objLoader;
        if (0)
            objLoader = putar::createMyLoader();
        else{
            objLoader = putar::createMy3dsLoader(Loader3dsConfig);
        }
//        objLoader->attachVisualizer(&visu);
        objLoader->loadObj();

        ImageVisualizer* visu2D = putar::createMyImageVisualizer("ImageVisualizerConfig.xml");


        Hmi* hmiDev = putar::createMyHmiGamepad("HmiGamepadConfig.xml");

//        application.exec();

        std::thread processThr(processSLAM, &slam);

        std::thread putarThr(processPUTAR, objLoader, visu2D, hmiDev);



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
