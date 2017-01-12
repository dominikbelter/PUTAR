// based on the Marek Kopicki's code (boost server)

#include "RPC/serverTest.h"
#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <thread>


void requestHandler(putar::Stream& stream) {
    // first 4 bytes encode request type
    std::uint32_t cmd;
    putar::StreamRead(stream, cmd);
    // respond to request
    switch (cmd) {
    case HandlerRPC::REQ_CONST:
    {
        double command;
        putar::StreamRead(stream,command);
        putar::StreamWrite(stream, 3.14);
        break;
    }
    case HandlerRPC::REQ_ECHO:
    {
        double command;
        putar::StreamRead(stream,command);
        putar::StreamWrite(stream, command);
        break;
    }
    default:
        throw std::runtime_error("Unknown request: " + std::to_string(cmd));
    }
}

/// server thread
void serverRunning(unsigned short int port){
    // run server
    putar::Server server(port);
    for (;;) {
        // wait for incomming connection
        std::cout << "Listening on port: " << port << "\n";
        putar::Stream::Ptr stream = server.accept<putar::StreamSocket>();

        // debug info
        const boost::asio::ip::tcp::socket& socket = static_cast<const putar::StreamSocket&>(*stream).getSocket();
        std::cout << "Accepted connection from host: " << socket.remote_endpoint().address().to_string() << ", port: " << socket.remote_endpoint().port() << "\n";

        try {
            // handle requests
            for (;;) requestHandler(*stream);
        }
        catch (const std::exception& ex) {
            fprintf(stderr, "%s\n", ex.what());
        }
    }
}

//------------------------------------------------------------------------------
void HandlerRPC::Config::load(std::string configFilename){
    tinyxml2::XMLDocument configSrv;
    configSrv.LoadFile(std::string("../../resources/" + configFilename).c_str());
    if (configSrv.ErrorID())
        std::cout << "unable to load server config file.\n";
    configSrv.FirstChildElement("Server")->FirstChildElement("parameters")->QueryUnsignedAttribute("port",&port);
}

int main(void) {
    try {
        HandlerRPC::Config configServer;
        configServer.load("putarRPC.xml");

        serverRunning((unsigned short int) configServer.port);
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "%s\n", ex.what());
        return 1;
    }

    return 0;
}
