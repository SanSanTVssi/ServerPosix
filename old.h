//
// Created by aai2002 on 12/25/21.
//

#ifndef SERVER_OLD_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "logging.h"

using uint256 = unsigned long;;
const uint256 defaultPort = 4444;

void server() {
    int serverFD = socket(AF_INET,SOCK_STREAM,0);
    if(!serverFD) {
        logging::ThrowError("Socket creation failed!");
    }
    sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(defaultPort);
    address.sin_family = AF_INET;
    socklen_t addressLength = sizeof(address);
    if (bind(serverFD, (struct sockaddr*)&address, addressLength)) {
        logging::ThrowError("Socket bind failed!");
    }
    logging::WriteLine("Server start listening...");
    if (listen(serverFD, 1)) {
        logging::ThrowError("Error while listening!");
    }
    int clientFD = accept(serverFD, (struct sockaddr*)&address, &addressLength);
    if(!clientFD) {
        logging::WriteLine("Client connection failed");
        return;
    }
    int readLength;
    char buffer[1024] = {0};
    readLength = read(clientFD, buffer, 1024);
    logging::WriteLine(buffer);
}

#endif //SERVER_OLD_H
