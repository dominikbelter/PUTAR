#ifndef HANDLER_MOTION_H_INCLUDED
#define HANDLER_MOTION_H_INCLUDED

#include "rpc.h"
#include <iostream>
#include "../../3rdParty/tinyXML/tinyxml2.h"

using namespace putar;

class HandlerRPC {
    public:

        class Config
        {
            public:

                Config()
                {
                }

                Config(std::string configFilename)
                {
                    load(configFilename);
                }

                void load(std::string configFilename);

                /// port no
                unsigned int port;
        };

        HandlerRPC(std::string filename);

        ~HandlerRPC();

        Config config;

        enum Req : std::uint32_t {
            REQ_ECHO = 0,
            REQ_CONST = 1,
        };

};

#endif
