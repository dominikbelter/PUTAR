#ifndef _RPC_INTERFACE_H_ // if #pragma once is not supported
#define _RPC_INTERFACE_H_

//------------------------------------------------------------------------------

#include "Defs/defs.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>

//------------------------------------------------------------------------------

/** putar name space */
namespace putar {

    class Stream {
    public:
        /** Pointer */
        typedef std::shared_ptr<Stream> Ptr;

        /** Read from stream */
        virtual void read(size_t size, void* data) = 0;
        /** Write to stream */
        virtual void write(size_t size, const void* data) = 0;
    };

    template <typename _Type> void StreamWrite(Stream& stream, const _Type& data) {
        stream.write(sizeof(_Type), &data);
    }
    template <typename _Type> void StreamRead(Stream& stream, _Type& data) {
        stream.read(sizeof(_Type), &data);
    }

    /*************************************************************************
    *
    * Client-server communication
    *
    **************************************************************************/

    class Server;
    class Client;

    /** Synchronous socket interface */
    class StreamSocket : public Stream{
    public:
        friend class Server;
        friend class Client;

        /** Read from stream */
        void read(size_t size, void* data);
        /** Write to stream */
        void write(size_t size, const void* data);

        /** Get socket */
        const boost::asio::ip::tcp::socket& getSocket() const {
            return socket;
        }

        /** Closes connection */
        ~StreamSocket() {}

    protected:
        StreamSocket(boost::asio::ip::tcp::socket& socket) : socket(std::move(socket)) {}

        boost::asio::ip::tcp::socket socket;
    };

    /** Asynchronous socket interface */
    class StreamAsyncSocket {
    public:
        friend class Server;
        friend class Client;

        /** Read from stream */
        virtual void read(size_t size, void* data) {
            boost::asio::async_read(
                socket,
                boost::asio::buffer(data, size),
                boost::asio::transfer_all(),
                //boost::asio::transfer_exactly(size),
                boost::bind(
                    &StreamAsyncSocket::readHandler,
                    boost::asio::placeholders::error
                )
            );

            while (socket.get_io_service().run_one());
            socket.get_io_service().reset();
        }
        /** Write to stream */
        virtual void write(size_t size, const void* data) {
            boost::asio::async_write(
                socket,
                boost::asio::buffer(data, size),
                boost::asio::transfer_all(),
                boost::bind(
                    &StreamAsyncSocket::writeHandler,
                    boost::asio::placeholders::error
                )
            );

            while (socket.get_io_service().run_one());
            socket.get_io_service().reset();
        }

        /** Get socket */
        const boost::asio::ip::tcp::socket& getSocket() const {
            return socket;
        }

        /** Closes connection */
        ~StreamAsyncSocket() {}

    protected:
        static void readHandler(const boost::system::error_code& error) {
            if (error)
                throw std::runtime_error("StreamAsyncSocket::readHandler(): unable to send data: " + error.message());
        }
        static void writeHandler(const boost::system::error_code& error) {
            if (error)
                throw std::runtime_error("StreamAsyncSocket::writeHandler(): unable to send data: " + error.message());
        }

        StreamAsyncSocket(boost::asio::ip::tcp::socket& socket) : socket(std::move(socket)) {}

        boost::asio::ip::tcp::socket socket;
    };

    /** TCP/IP server */
    class Server {
    public:
        /** Create server, listen on specified port */
        Server(const unsigned short port) : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

        /** Accept incoming connection */
        template <typename _Stream> typename _Stream::Ptr accept() {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);
            return typename _Stream::Ptr(new _Stream(socket));
        }

    protected:
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::acceptor acceptor;
    };

    /** TCP/IP client */
    class Client {
    public:
        /** Create client */
        Client() {}

        /** Initialise connection */
        template <typename _Stream> typename _Stream::Ptr connect(const std::string& host, const unsigned short port/*, unsigned timeoutMs = 10000*/) {
            boost::asio::ip::tcp::socket socket(io_service);
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, std::to_string(port));

            boost::system::error_code error;

            // IPv4 first
            std::vector<boost::asio::ip::tcp::resolver::iterator> endpoints;
            for (boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, error), end; endpoint != end; ++endpoint)
                endpoints.insert(endpoint->endpoint().address().is_v4() ? endpoints.begin() : endpoints.end(), endpoint);

            // try all endpoints
            for (std::vector<boost::asio::ip::tcp::resolver::iterator>::const_iterator endpoint = endpoints.begin(); endpoint != endpoints.end(); ++endpoint) {
                socket.connect(**endpoint, error);

                if (!error)
                    return typename _Stream::Ptr(new _Stream(socket));

                socket.close();
            }
            throw boost::system::system_error(error);
        }

    protected:
        boost::asio::io_service io_service;
    };
} // namespace putar

//------------------------------------------------------------------------------

#endif // _RPC_INTERFACE_H_
