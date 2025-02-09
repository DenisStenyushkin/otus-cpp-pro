#pragma once

#include <string>
#include <vector>

#include "database.hpp"


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

class CommandHandler {
public:
    std::string handle(std::string command) {
        try {
            std::vector<std::string> tokens = split(command, ' ');
            if (tokens.empty()) {
                return "No-op";
            }

            if (tokens[0] == "INSERT") {
                if (tokens.size() != 4) {
                    return "Usage: INSERT <table> <id> <name>";
                }

                db_.insert(tokens[1], std::stoi(tokens[2]), tokens[3]);
                return "OK";
            }
            else if (tokens[0] == "TRUNCATE") {
                if (tokens.size() != 2) {
                    return "Usage: TRUNCATE <table>";
                }

                db_.truncate(tokens[1]);
                return "OK";
            }
            else if (tokens[0] == "INTERSECTION") {
                if (tokens.size() != 1) {
                    return "Usage: INTERSECTION";
                }

                auto result = db_.intersection();
                
                std::stringstream response;
                for (auto& [id, name1, name2]: result) {
                    response << id << "," << name1 << "," << name2 << std::endl;
                }
                response << "OK";

                return response.str();
            }
            else if (tokens[0] == "SYMMETRIC_DIFFERENCE") {
                if (tokens.size() != 1) {
                    return "Usage: SYMMETRIC_DIFFERENCE";
                }

                auto result = db_.symmetric_difference();
                
                std::stringstream response;
                for (auto& [id, name1, name2]: result) {
                    response << id << "," << name1 << "," << name2 << std::endl;
                }
                response << "OK";

                return response.str();
            }
            else {
                std::stringstream response;
                response << "Unknown operation: " << tokens[0];
                return response.str();
            }
        } 
        catch (const std::exception& ex) {
            std::stringstream response;
            response << "Unexpected error: " << ex.what();
            return response.str();
        }
    }

private:
    Database db_;
};
