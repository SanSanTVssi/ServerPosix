//
// Created by aai2002 on 26.12.21.
// * Interface for Server
// * (can be use for all realisation of server
// * but in this proj it use only for posix server)
//

#ifndef SERVER_ISERVER_H
#define SERVER_ISERVER_H

#include <iostream>

namespace my_std {
    class IServer {
    public:
        virtual void startListening() = 0;
        virtual void addClient() = 0;
        virtual void deleteClient() = 0;
        virtual std::string read(std::size_t clientId = 0) = 0;
        virtual void send(const std::string&, std::size_t clientId = 0) = 0;
        virtual void acceptfile(const std::string& outFilename, std::size_t clientId = 0) = 0;
        virtual std::size_t getClientsCount() const = 0;
        virtual ~IServer() = default;
    };
}

#endif //SERVER_ISERVER_H
