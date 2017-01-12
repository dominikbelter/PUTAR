
#include "RPC/rpc.h"

using namespace putar;

/** Read from stream */
void StreamSocket::read(size_t size, void* data) {
    boost::system::error_code error;
    boost::asio::read(
        socket,
        boost::asio::buffer(data, size),
        boost::asio::transfer_all(),
        error
    );
    if (error)
        throw std::runtime_error("StreamSocket::read(): unable to receive data: " + error.message());
}

/** Write to stream */
void StreamSocket::write(size_t size, const void* data) {
    boost::system::error_code error;
    boost::asio::write(
        socket,
        boost::asio::buffer(data, size),
        boost::asio::transfer_all(),
        error
    );
    if (error)
        throw std::runtime_error("StreamSocket::write(): unable to send data: " + error.message());
}
