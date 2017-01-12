#include "RPC/clientRPC.h"

using namespace putar;
using namespace std;


ClientRPC::ClientRPC(void){
    initRPC();
}

ClientRPC::~ClientRPC(void) {
}

ClientRPC::ClientRPC(std::string configFilename){
    config.load(configFilename);
    initRPC();
}

///cend const
void ClientRPC::sendConst(double request){
    putar::StreamWrite(*streamRPC, (std::uint32_t)HandlerRPC::REQ_CONST);
    putar::StreamWrite(*streamRPC, request);
    double response;
    putar::StreamRead(*streamRPC, response);
    std::cout << "received: " << response << "\n";
}

///Send echo
void ClientRPC::sendEcho(double request){
    putar::StreamWrite(*streamRPC, (std::uint32_t)HandlerRPC::REQ_ECHO);
    putar::StreamWrite(*streamRPC, request);
    double response;
    putar::StreamRead(*streamRPC, response);
    std::cout << "received: " << response << "\n";
}


void ClientRPC::initRPC() {
    streamRPC = client.connect<putar::StreamSocket>(config.host, (short unsigned int)config.port);
}

void ClientRPC::closeServer() {
}
