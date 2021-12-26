//
// Created by aai2002 on 12/25/21.
//

#include "server.h"

my_std::posixServer::posixServer(in_addr_t ip, int connectionType, in_port_t port, sa_family_t family, int protocol, std::size_t bufferSize)
: _address(ip, port, family), _isWorking(false), _buffer(bufferSize, defVal)
{
    _serverFD = socket(family, connectionType, protocol);
    if(!_serverFD) {
        throw std::runtime_error("Socket creation failed!");
    }
    logging::WriteTimedLine("The server started up");

}

my_std::posixServer::~posixServer() {
    logging::WriteTimedLine("The server has finished working ");
    closefile(_serverFD);
    for(filedesc_t clientFD : _clientFDs) {
        closefile(clientFD);
    }
    _isWorking = false;
}

void my_std::posixServer::bind() {
    if (::bind(_serverFD, _address.getSockaddrPtr(), _address.length())) {
        throw std::runtime_error("Socket bind failed!");
    }
}

void my_std::posixServer::listen(){
    if (::listen(_serverFD, 1)) {
        throw std::runtime_error("Error while listening!");
    }
}

void my_std::posixServer::startListening() {
    if(!_isWorking) {
        bind();
        listen();
        logging::WriteTimedLine("Server start listening");
        _isWorking = true;
    }
}

std::string my_std::posixServer::read(std::size_t clientId) {
    if(!_isWorking) return {};
    filedesc_t _clientFD = _clientFDs[clientId];
    std::size_t size;
    std::size_t readLen = ::read(_clientFD, &size, sizeof(size));
    if (readLen == 0) { return {}; }
    if(size > _buffer.size()) {
        throw std::runtime_error("Message read failed. Message too big");
    }
    //char * buffer = new char[size + 1] {0};
    readLen = ::read(_clientFD, _buffer.getBuf(), size);
    if (readLen == 0) { return {}; }
    std::string temp (_buffer.getBuf());
    logging::WriteTimedLine("Read message:\t" + temp);
    _buffer.zeroingBuffer();
    return temp;
}

void my_std::posixServer::send(const std::string& message, std::size_t clientId, int flags) {
    if(!_isWorking) return;
    filedesc_t _clientFD = _clientFDs[clientId];
    auto size = message.size();
    ::send(_clientFD, &size, sizeof(size), flags);
    ::send(_clientFD, message.c_str(), size, flags);
    logging::WriteTimedLine("Send message:\t" + message);
}

void my_std::posixServer::addClient() {
    if(!_isWorking) return;
    filedesc_t clientFD = accept(_serverFD, _address.getSockaddrPtr(), _address.getLenPtr());
    if(!clientFD) {
        logging::WriteTimedLine("Client connection failed");
    }
    logging::WriteTimedLine("Client connection succeed");
    _clientFDs.push_back(clientFD);
}

void my_std::posixServer::deleteClient() {
    if(!_isWorking) return;
    long flagDisconnected;
    ::read(_clientFDs[0], &flagDisconnected, sizeof(flagDisconnected));
    if(flagDisconnected == 0xD) {
        closefile(_clientFDs[0]);
        _clientFDs.pop_front();
        logging::WriteTimedLine("Client was disconnected");
    }
}

void my_std::posixServer::acceptfile(const std::string &outFilename, std::size_t clientId) {
    acceptfile(outFilename, clientId, 0);
}

void my_std::posixServer::acceptfile(const std::string &outFilename, std::size_t clientId, int flags)  {
    logging::WriteTimedLine("start accept into " + outFilename);
    filedesc_t file;
    if ((file = open(outFilename.c_str(), O_WRONLY)) != 0) {
        file = creat(outFilename.c_str(), O_WRONLY);
    }

    acceptfile(file, clientId, flags);
}

void my_std::posixServer::acceptfile(my_std::filedesc_t outfile, std::size_t clientId, int flags) {
    filedesc_t clientFD = _clientFDs[clientId];
    off_t fsize;
    ::read(clientFD, &fsize, 1*sizeof(off_t));
    const auto bufsize = static_cast<off_t>(_buffer.size()*sizeof(_buffer.getDefVal()));
    while(fsize) {
        off_t readLen = ::read(clientFD, _buffer.getBuf(), bufsize);
        off_t writeLen = write(outfile, _buffer.getBuf(), readLen);
        if(writeLen == -1 && readLen == -1) {
            closefile(outfile);
            throw std::runtime_error("File accepting failed!");
        }
        fsize -= readLen;
    }
    closefile(outfile);
    logging::WriteTimedLine("finish accept file");
}
