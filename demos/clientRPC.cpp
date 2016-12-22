#include "Defs/defs.h"
#include "RPC/clientRPC.h"
#include <iostream>
#include <thread>

using namespace std;


int main(void)
{
    try {
        ClientRPC client("putarRPC.xml");
        //client.initRPC();
        while(1){
            std::uint32_t order;
            std::cout << "Set order no (0-const, 1-echo): ";
            std::cin >> order;
            std::cout << "\nsend command: ";
            double request;
            std::cin >> request;
            if (order==0)
                client.sendConst(request);
            else if (order==1)
                client.sendEcho(request);
        }
        client.closeServer();
        std::cout << "Finished\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
