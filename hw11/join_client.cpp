#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "message.hpp"


class JoinClient {
public:
    JoinClient(boost::asio::io_context& io_context,
               boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    : io_context_{io_context}, socket_{io_context} {
        do_connect(endpoint_iterator);
    }

    void write(const Message& msg) {
        io_context_.post([this, msg]() {
                            bool write_in_progress = !write_msgs_.empty();
                            write_msgs_.push_back(msg);
                            if (!write_in_progress) {
                                do_write();
                            }
                        });
    }

    void close() {
        io_context_.post([this]() { socket_.close(); });
    }

private:
    void do_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
        boost::asio::async_connect(socket_, endpoint_iterator,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator) {
            if (!ec) {
                do_read_header();
            }
        });
    }

    void do_read_header() {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), Message::header_length),
                                [this](boost::system::error_code ec, size_t) {
                                    if (!ec && read_msg_.decode_header()) {
                                        do_read_body();
                                    }
                                    else {
                                        socket_.close();
                                    }
                                });
    }

    void do_read_body() {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this](boost::system::error_code ec, size_t) {
                                    if (!ec) {
                                        std::cout.write(read_msg_.body(), read_msg_.body_length());
                                        std::cout << std::endl;
                                        do_read_header();
                                    }
                                    else {
                                        socket_.close();
                                    }
                                });
    }

    void do_write() {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this](boost::system::error_code ec, size_t) {
                                    if (!ec) {
                                        write_msgs_.pop_front();
                                        if (!write_msgs_.empty()) {
                                            do_write();
                                        }
                                    }
                                    else {
                                        socket_.close();
                                    }
                                 });
    }

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    Message read_msg_;
    std::deque<Message> write_msgs_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: join_client <host> <port>";
            return 1;
        }

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver{io_context};
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
        JoinClient c{io_context, endpoint_iterator};

        std::thread t{[&io_context]() { io_context.run(); }};

        char line[Message::max_body_length + 1];
        while(std::cin.getline(line, Message::max_body_length + 1)) {
            Message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
