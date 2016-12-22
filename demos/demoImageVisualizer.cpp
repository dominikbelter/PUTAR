#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <thread>
#include "ImageVisualizer/imageVisualizerCV.h"
#include "ImageVisualizer/imageVisualizer.h"

using namespace std;

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
    ImageVisualizer* x = putar::createMyImageVisualizer();

    x->UpdateImage();

    return 0;
}

