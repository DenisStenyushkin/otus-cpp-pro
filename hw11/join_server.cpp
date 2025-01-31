#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "boost/asio.hpp"

#include "command_handler.hpp"
#include "message.hpp"


class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket) : socket_{std::move(socket)} {}

    void start() {
        do_read_header();
    }

private:
    void do_read_header() {
        auto self{shared_from_this()};
        read_msg_.clear();
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), Message::header_length),
                                [this, self](boost::system::error_code ec, size_t) {
                                    if (!ec && read_msg_.decode_header()) {
                                        do_read_body();
                                    }
                                });
    }

    void do_read_body() {
        auto self{shared_from_this()};
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this, self](boost::system::error_code ec, size_t) {
                                    if (!ec) {
                                        std::cout << "Received: " << read_msg_.body() << std::endl;
                                        std::string response = cmd_.handle(read_msg_.body());
                                        send_response(response.c_str());

                                        do_read_header();
                                    }
                                });
    }

    void send_response(const char* body) {
        Message msg;
        msg.body_length(std::strlen(body));
        std::memcpy(msg.body(), body, msg.body_length());
        msg.encode_header();
        do_write(msg);
    }

    void do_write(Message send_msg) {
        auto self{shared_from_this()};
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(send_msg.data(), send_msg.length()),
                                 [this](boost::system::error_code ec, size_t){
                                    if (ec) {
                                        std::cerr << "Failed to send message" << std::endl;
                                    }
                                 });
    }

    boost::asio::ip::tcp::socket socket_;
    char data_[Message::header_length + Message::max_body_length];
    Message read_msg_;
    CommandHandler cmd_;
};

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port)
        : acceptor_{io_context, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}}
        {
            do_accept();
        }
private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            }
        );
    }

    boost::asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: join_server <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        Server server{io_context, static_cast<unsigned short>(std::atoi(argv[1]))};

        io_context.run();
    }
    catch(const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
