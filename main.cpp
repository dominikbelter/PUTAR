#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <thread>

//void processSLAM(PUTSLAM* slam){
//    slam->process();
//}

int main()
{
    try {
        using namespace putar;
        using namespace std;

        tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";
        //std::string simConfig = config.FirstChildElement( "Environment" )->Attribute("config");

        ObjLoader* objLoader = putar::createMyLoader();

        objLoader->loadObj("kamien.obj");

        //PUTSLAM slam;

        ImageVisualizer* visu2D = putar::createMyImageVisualizer();

        //std::thread processThr(processSLAM, &slam);

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

        //processThr.join();

        std::cout << "Done\n";
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
