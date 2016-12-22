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


//// The width and height of your window, change them as you like
int screen_width=640;
int screen_height=480;


//Now the object is generic, the cube has annoyed us a little bit, or not?
obj_type object;


void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D){
    while(1){
        Mat34 camPose;
        //slam.getPose(camPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        //slam.getFrame(rgbImg, depthImg);
        cv::Mat mask;
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

        //objLoader->loadObj("stone.obj");
        putar::obj_type object;

        objLoader->loadObj();

        objLoader->getMesh(object);

///////////////////////////////////////////////////////////////////////////////////////////////////////

        cv::Mat dst;
        Mat34 cameraPose;
        GLfloat depth[640*480];

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(screen_width,screen_height);
        glutInitWindowPosition(0,0);
        glutCreateWindow("At the moment unfortunately this window is nesessery");

        objLoader->computeMask(cameraPose, dst, depth);

        for (int i=0; i<depth.length; i++)
        {
            cout<<depths[i]<<endl;
        }


        cv::namedWindow("imgMAT");
        cv::imshow("imgMAT", dst);

        cv::waitKey(1000);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "Done\n";
}
catch (const std::exception& ex) {
std::cerr << ex.what() << std::endl;
return 1;
}

return 0;
}
