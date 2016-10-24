#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include <GL/glut.h>
#include <QtWidgets/QApplication>
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char** argv)
{
    try {
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

        application.exec();
        std::cout << "Finished\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
