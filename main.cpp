#include "Defs/defs.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include <iostream>

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

        std::cout << "Done\n";
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
