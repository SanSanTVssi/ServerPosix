//
// Created by aai2002 on 12/25/21.
// Server implementation for work in unix(posix) systems
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "forward_list.h"
#include "base.h"
#include "IServer.h"

#include <sys/sendfile.h>

namespace my_std {

    /**
     * Server implementation for work in unix(posix) systems
     */
    class posixServer final : public IServer{
    private:
        address _address;
        filedesc_t _serverFD;
        bool _isWorking;
        Buffer _buffer;
        forward_list<filedesc_t> _clientFDs;
    public:

        explicit posixServer(in_addr_t ip = INADDR_ANY,
                    int connectionType = SOCK_STREAM,
                    in_port_t port = defaultPort,
                    sa_family_t family = AF_INET,
                    int protocol = 0,
                    std::size_t bufferSize = DEFAULT_MAX_BUFFER_SIZE);

        ~posixServer() final;

        posixServer(const posixServer& other) = delete;
        posixServer& operator=(const posixServer& other) = delete;

        std::size_t getClientsCount() const final { return _clientFDs.size(); }

    private:
        void bind();
        void listen();
    public:
        /**
         * Start server
         */
        void startListening() final;

        /**
         * Add new client to server
         * (required pool for work with several clients)
         */
        void addClient() final;
        /**
         * Delete client from pool before client disconnection
         */
        void deleteClient() final;
        /**
         * Read message from client
         * @param clientId - id of client in pool
         * @return message
         */
        std::string read(std::size_t clientId = 0) final;
        /**
         * Send message for client
         * @param message - message
         * @param clientId - id of client in pool
         */
        void send(const std::string& message, std::size_t clientId = 0) final { send(message, clientId, 0); }
        void send(const std::string& message, std::size_t clientId = 0, int flags = 0);

        /**
         * Accept files from client
         * @param outFilename - filename for write into data
         * @param clientId - client-sender
         */
        void acceptfile(const std::string& outFilename, std::size_t clientId = 0) final;
        void acceptfile(const std::string& outFilename, std::size_t clientId, int flags);
        void acceptfile(filedesc_t outfile, std::size_t clientId,  int flags);
    };
}

#endif //SERVER_SERVER_H
