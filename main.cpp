#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <PUTSLAM/PUTSLAM.h>
#include "Utilities/offlineSLAM.h"
#include <GL/glut.h>
#include <QApplication>
#include <iostream>
#include <thread>

//PUTSLAM slam;

//void processSLAM(PUTSLAM* _slam){
//    _slam->startProcessing();
//}
//test

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
        objLoader->loadObj();
        putar::obj_type object;

        //ImageVisualizer* visu2D = putar::createMyImageVisualizer("ImageVisualizerConfig.xml");

        Hmi* hmiDev = putar::createMyHmiGamepad("HmiGamepadConfig.xml");

        OfflineSLAM offlineSLAM("offlineSLAM.xml");

        //std::thread slamThr(processSLAM, &slam);

        //application.exec();

        //std::thread putarThr(processPUTAR, objLoader, hmiDev);//, visu2D);

        //slamThr.join();
        //putarThr.join();
        std::cout << "putar start\n";
        usleep(1000000);
        std::cout << "putar started\n\n\n\n\n\n\n\n\n";
        Mat34 prevCameraPose(Mat34::Identity());
        while(1){


    //        slam.getCurrentPose(camPose);
    //        slam.getCurrentFrame(rgbImg, depthImg);
    //        cv::imshow("putar rgb",rgbImg);
    //        cv::waitKey(30);

            //slam.getCurrentPose(camPose);
            cv::Mat rgbImg, depthImg;
            std::cout << "get frame\n";
            offlineSLAM.getFrame(rgbImg, depthImg);
            cv::namedWindow("putar rgb");
            cv::imshow("putar rgb",rgbImg);
            cv::waitKey(3);
//            cv::namedWindow("putar depth");
//            cv::imshow("putar depth",depthImg);
//            cv::waitKey(30);
            std::cout << "get frame end\n";

            Mat34 objPose;
            hmiDev->updatePose(objPose);

            Mat34 cameraPose(Mat34::Identity());
            offlineSLAM.getCurrentPose(cameraPose);


            cv::Mat rgbMask, depthMask;
            objLoader->getMesh(object);

            objLoader->computeMask(cameraPose.inverse(), objPose, rgbMask, depthMask);
            //objLoader->computeMask(cameraPose, rgbMask, depthMask);

            cv::namedWindow("mask");
            cv::imshow("mask",rgbMask);
            //cv::waitKey(30);
            cv::namedWindow("depthmask");
            cv::imshow("depthmask",depthMask);
            cv::waitKey(2);
            /*visu2D->updateMask(rgbMask, depthMask);
            visu2D->updateFrame(rgbImg,depthImg);*/
            usleep(3000);
            prevCameraPose = cameraPose;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
