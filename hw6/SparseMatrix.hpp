#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>


template<typename TVal, TVal default_value>
class SparseMatrix {
public:
    struct SparseElement {
        std::size_t i;
        std::size_t j;
        TVal value;

        SparseElement(std::size_t ii, std::size_t jj, TVal vv) : i{ii}, j{jj}, value{vv} { }

        operator std::tuple<std::size_t, std::size_t, TVal>() const {
            return std::make_tuple(i, j, value);
        }

        operator TVal() const {
            return value;
        }

        template<typename T>
        SparseElement& operator=(const T& v) {
            value = v;
            return *this;
        }
    };

    class RowIndexProxy {
    public:
        RowIndexProxy(SparseMatrix& matrix, std::size_t i) : m_matrix{matrix}, m_i{i} {}

        SparseElement& operator[](std::size_t j) {
            return m_matrix.at(m_i, j);
        }
    private:
        SparseMatrix& m_matrix;
        std::size_t m_i;
    };

    SparseMatrix() = default;
    SparseMatrix(const SparseMatrix<TVal, default_value>& other) = delete;
    SparseMatrix(const SparseMatrix<TVal, default_value>&& other) = delete;

    SparseMatrix<TVal, default_value>& operator=(SparseMatrix<TVal, default_value>& other) = delete;
    SparseMatrix<TVal, default_value>& operator=(SparseMatrix<TVal, default_value>&& other) = delete;

    RowIndexProxy operator[](std::size_t i) {
        prune();
        return RowIndexProxy(*this, i);
    }

    std::size_t size() const {
        prune();
        return m_data.size();
    }

    auto begin() {
        prune();
        return m_data.begin();
    }

    auto end() {
        prune();
        return m_data.end();
    }
private:
    mutable std::vector<SparseElement> m_data;

    void prune() const {
        m_data.erase(std::remove_if(m_data.begin(), m_data.end(),
                                    [](SparseElement& elem) { return elem.value == default_value; }),
                     m_data.end());
    }

    SparseElement& at(std::size_t i, std::size_t j) {
        auto elem = std::find_if(m_data.begin(), m_data.end(),
                                 [i, j](SparseElement& e) { return (e.i == i) && (e.j == j); });
        if (elem == m_data.end()) {
            return m_data.emplace_back(i, j, default_value);
        }
        return *elem;
    }
};
