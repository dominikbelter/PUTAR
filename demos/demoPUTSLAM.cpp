#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include <GL/glut.h>
#include <QApplication>
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


        QApplication application(argc,argv);
        setlocale(LC_NUMERIC,"C");
        glutInit(&argc, argv);

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
