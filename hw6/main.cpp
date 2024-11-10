#include <iostream>
#include "SparseMatrix.hpp"


int main() {
    SparseMatrix<int, 0> matrix;

    for (std::size_t i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9-i] = 9 - i;
    }

    std::size_t min_row = 1, max_row = 8;
    std::size_t min_col = 1, max_col = 8;
    for (std::size_t i = min_row; i <= max_row; ++i) {
        for (std::size_t j = min_col; j <= max_col; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << matrix.size() << std::endl;
}
