#include <iostream>
#include <string>

#include "CommandProcessor.h"
#include "OutputHandler.hpp"


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "The program accepts exaxtly one integral argument" << std::endl;
        std::cout << "Example: ./bulk 3" << std::endl;
        return -1;
    }

    auto processor = std::shared_ptr<CommandProcessing::CommandProcessor>(
        new CommandProcessing::CommandProcessor{
            std::make_shared<OutputHandling::ConsoleOutputHandler>()
        }
    );

    auto processor_state_fabric = std::make_shared<CommandProcessing::CommandProcessorStateFabric>(
        processor, std::stoi(argv[1])
    );

    processor->SwitchTo(processor_state_fabric->MakeBatch());

    for(std::string line; std::getline(std::cin, line);)
    {
        processor->HandleCommand(line);
    }
}