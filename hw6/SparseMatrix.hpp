#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>


template<typename TVal, TVal default_value>
class SparseMatrix {
public:
    using TRow = std::unordered_map<std::size_t, TVal>;
    using TMatrix = std::unordered_map<std::size_t, TRow>;

    class SparseRow {
    public:
        SparseRow(TRow& row) : m_data{row} {}

        TVal& operator[](std::size_t j) {
            return access(j);
        }

        const TVal& operator[](std::size_t j) const {
            return access(j);
        }
    private:
        TRow& m_data;

        TVal& access(std::size_t j) {
            if (m_data[j] == TVal{}) {
                m_data[j] = default_value;
            }
            return m_data[j];
        }
    };

    // struct Iterator
    // {
    //     using iterator_category = std::input_iterator_tag;
    //     using difference_type = std::ptrdiff_t;
    //     using value_type = std::tuple<std::size_t, std::size_t, TVal>;
    //     using pointer = value_type*;
    //     using reference = value_type&;

    //     Iterator(const TMatrix& matrix) : m_matrix{matrix} {
    //         m_over_rows_iterator = m_matrix.begin();
    //         if (m_over_rows_iterator != m_matrix.end()) {
    //             m_over_cols_iterator = m_over_rows_iterator->begin();
    //         }
    //         else {
    //             m_over_cols_iterator = nullptr;
    //         }
    //     }
    // private:
    //     const TMatrix& m_matrix;
    //     typename TMatrix::iterator m_over_rows_iterator;
    //     typename TRow::iterator m_over_cols_iterator;
    // };

    SparseMatrix() = default;
    SparseMatrix(const SparseMatrix<TVal, default_value>& other) = delete;
    SparseMatrix(const SparseMatrix<TVal, default_value>&& other) = delete;

    SparseMatrix<TVal, default_value>& operator=(SparseMatrix<TVal, default_value>& other) = delete;
    SparseMatrix<TVal, default_value>& operator=(SparseMatrix<TVal, default_value>&& other) = delete;

    SparseRow operator[](std::size_t i) {
        prune();
        return SparseRow(m_data[i]);
    }

    std::size_t size() const {
        prune();

        // TODO: with STL Algorithms
        std::size_t size = 0;
        for (auto& [row_idx, row]: m_data) {
            if (row.size() == 0) {
                throw "Empty row found undeleted";
            }
            size += row.size();
        }
        return size;
    }

    Iterator begin() {
        return Iterator(m_data);
    }

    Iterator end() {}
private:
    mutable TMatrix m_data;

    void prune() const {
        // TODO: with STL Algorithms
        std::vector<std::size_t> rowsToDelete;
        for (auto& [row_num, row]: m_data) {
            std::vector<std::size_t> elemsToDelete;
            for (auto& [col_num, elem]: row) {
                if (elem == default_value) {
                    elemsToDelete.push_back(col_num);
                }
            }
            for (std::size_t idx: elemsToDelete) {
                row.erase(idx);
            }

            if (row.empty()) {
                rowsToDelete.push_back(row_num);
            }
        }
        for (std::size_t idx: rowsToDelete) {
                m_data.erase(idx);
        }
    }
};
