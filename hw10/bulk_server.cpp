#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "CommandProcessor.h"
#include "CommandsStorage.hpp"
#include "OutputHandler.hpp"

#include "boost/asio.hpp"


class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket, size_t bulk_size,
            std::shared_ptr<CommandProcessing::CommandsStorage> batch_storage) : socket_{std::move(socket)} {
        processor_ = std::shared_ptr<CommandProcessing::CommandProcessor>(
            new CommandProcessing::CommandProcessor{
                std::make_shared<OutputHandling::ConsoleOutputHandler>(),
                std::make_shared<OutputHandling::FileOutputHandler>()
            }
        );

        batch_storage->commands.clear();
        state_fabric_ = std::make_shared<CommandProcessing::CommandProcessorStateFabric>(
            processor_, bulk_size, batch_storage
        );

        processor_->SwitchTo(state_fabric_->MakeBatch());
    }

    void start() {
        do_read();
    }

private:
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

    void do_read() {
        auto self{shared_from_this()};
        socket_.async_read_some(boost::asio::buffer(data_, buffer_size),
                                [this, self](boost::system::error_code ec, size_t length) {
                                    if (!ec) {
                                        std::string message{data_, length};
                                        std::cout << "Received: " << message;

                                        auto commands = split(message, '\n');
                                        for (const std::string& command: commands){
                                            processor_->HandleCommand(command);
                                        }
                                    }
                                });
    }

    boost::asio::ip::tcp::socket socket_;
    static const size_t buffer_size = 1024;
    char data_[buffer_size];

    std::shared_ptr<CommandProcessing::CommandProcessor> processor_;
    std::shared_ptr<CommandProcessing::CommandProcessorStateFabric> state_fabric_;
};

class Server {
public:
    Server(boost::asio::io_context& io_context, unsigned short port, size_t bulk_size)
        : acceptor_{io_context, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}},
          bulk_size_{bulk_size}
        {
            batch_storage_ = std::make_shared<CommandProcessing::CommandsStorage>();
            do_accept();
        }
private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket), bulk_size_, batch_storage_)->start();
                }
                do_accept();
            }
        );
    }

    boost::asio::ip::tcp::acceptor acceptor_;
    size_t bulk_size_;
    std::shared_ptr<CommandProcessing::CommandsStorage> batch_storage_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        Server server{io_context, static_cast<unsigned short>(std::atoi(argv[1])), 
                      static_cast<size_t>(std::atoi(argv[2]))};

        io_context.run();
    }
    catch(const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
