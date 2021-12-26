#include "logging.h"
#include "server.h"

using namespace my_std;

void ServerProcessing(IServer * server) {
    server->startListening();
    while(true) {
        server->addClient();
        auto message = server->read();
        if(message == "Hello") {
            server->send("world!");
        }
        else {
            server->send(message);
        }
        server->deleteClient();
    }
}

int main() {
    posixServer Server;
    try {
        ServerProcessing(&Server);
    }
    catch (const std::runtime_error& ex) {
        logging::WriteLine(ex.what());
    }
}
