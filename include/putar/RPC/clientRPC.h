#ifndef _ClientRPC_H
#define _ClientRPC_H

#include "RPC/serverTest.h"
#include <stdlib.h>
#include <thread>
#include <mutex>

class ClientRPC{
public:
    /// Pointer
    typedef std::unique_ptr<ClientRPC> Ptr;

    class Config
    {

      public:
        Config() {
        }

        Config(std::string configFilename){
            load(configFilename);
        }

        void load(std::string configFilename) {
            std::cout << "conf " << configFilename << "\n";
            tinyxml2::XMLDocument config;
            std::string filename = "../../resources/" + configFilename;
            config.LoadFile(filename.c_str());
            if (config.ErrorID())
                std::cout << "unable to load config file: " + filename << std::endl;

            host = std::string(config.FirstChildElement( "Server" )->FirstChildElement("parameters")->Attribute("host"));
            config.FirstChildElement("Server")->FirstChildElement( "parameters" )->QueryUnsignedAttribute("port", &port);
        }

        public:
            /// host name
            std::string host;
            /// port
            unsigned int port;
    };

    /// Construction
    ClientRPC(void);

    /// Destruction
    ~ClientRPC(void);

    /// Construction
    ClientRPC(std::string configFilename);

    ///constant response
    void sendConst(double request);

    ///Echo
    void sendEcho(double request);

    ///RPC Initialization
    void initRPC();

    ///RPC Exit
    void closeServer();

    Config config;

private:

    /// client
    putar::Client client;
    /// stream RPC
    putar::Stream::Ptr streamRPC;
};

#endif //_ClientRPC_H
