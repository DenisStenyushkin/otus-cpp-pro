#include <fstream>
#include <iostream>
#include <numeric>
#include <string>

#include "helpers.h"
#include "tf_classifier.h"


const size_t width = 28;
const size_t height = 28;
const size_t output_dim = 10;

using namespace mnist;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./tf_classifier <model_path> <test.csv_path> \n";
    }

    std::string model_path{argv[1]};
    std::string test_data_path{argv[2]};

    auto clf = TfClassifier{model_path, width, height};
    auto features = TfClassifier::features_t{};

    std::ifstream test_data{test_data_path};
    if (!test_data.is_open()) {
        std::cerr << "Unable to open file " << test_data_path << std::endl;
    }
    
    std::vector<char> matches;
    for (;;) {
        size_t y_true;
        test_data >> y_true;
        if (!read_features(test_data, features)) {
            break;
        }
        auto y_pred = clf.predict(features);
        matches.push_back(static_cast<char>(y_true == y_pred));
    }

    auto accuracy = static_cast<double>(std::accumulate(matches.begin(), matches.end(), 0)) / matches.size();
    std::cout << "Accuracy = " << accuracy << std::endl;
}
