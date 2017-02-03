#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <thread>
#include "ImageVisualizer/imageVisualizerCV.h"
#include "ImageVisualizer/imageVisualizer.h"
#include "ObjLoader/objLoader.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <QApplication>

using namespace std;
using namespace putar;
obj_type object;

int main(int argc, char** argv)
{
    /*
    try {
        tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";

        std::cout << "Finished\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    */

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
    visu.resize(640,480);
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

    objLoader->computeMask(cameraPose,objectPose, rgbMask, depthMask);

    cv::Mat RGB = cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/rgb_00752.png");
    cv::Mat RGBDepth = cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/depth_00752.png", cv::IMREAD_ANYDEPTH);

    ImageVisualizer* x = putar::createMyImageVisualizer();
    x->UpdateImage(rgbMask,depthMask,RGB,RGBDepth);



    cv::waitKey();
    return 0;
}

