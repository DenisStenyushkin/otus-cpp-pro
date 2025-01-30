#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "boost/asio.hpp"

#include "database.hpp"
#include "message.hpp"


std::vector<std::string> split(const std::string& s, const char delimiter) {
    std::stringstream sstream{s};
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(sstream, segment, delimiter))
    {
        seglist.push_back(segment);
    }

    return seglist;
}

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

                                        try {
                                            std::vector<std::string> tokens = split(read_msg_.body(), ' ');
                                            if (tokens.empty()) {
                                                return;
                                            }

                                            // TODO: Error handling!!!
                                            if (tokens[0] == "INSERT") {
                                                db_.insert(tokens[1], std::stoi(tokens[2]), tokens[3]);
                                                send_response("OK");
                                            }
                                            else if (tokens[0] == "TRUNCATE") {
                                                db_.truncate(tokens[1]);
                                                send_response("OK");
                                            }
                                            else if (tokens[0] == "INTERSECTION") {
                                                auto result = db_.intersection();
                                                
                                                std::stringstream response;
                                                for (auto& [id, name1, name2]: result) {
                                                    response << id << "," << name1 << "," << name2 << std::endl;
                                                }
                                                response << "OK";

                                                send_response(response.str().c_str());
                                            }
                                            else if (tokens[0] == "SYMMETRIC_DIFFERENCE") {
                                                auto result = db_.symmetric_difference();
                                                
                                                std::stringstream response;
                                                for (auto& [id, name1, name2]: result) {
                                                    response << id << "," << name1 << "," << name2 << std::endl;
                                                }
                                                response << "OK";

                                                send_response(response.str().c_str());
                                            }
                                            else {
                                                std::stringstream response;
                                                response << "Unknown operation: " << tokens[0];
                                                send_response(response.str().c_str());
                                            }
                                        } 
                                        catch (const std::exception& ex) {
                                            std::stringstream response;
                                            response << "Unexpected error: " << ex.what();
                                            send_response(response.str().c_str());
                                        }

                                        do_read_header();
                                    }
                                });
    }

    void send_response(const char* line) {
        Message msg;
        msg.body_length(std::strlen(line));
        std::memcpy(msg.body(), line, msg.body_length());
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
    Database db_;
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
