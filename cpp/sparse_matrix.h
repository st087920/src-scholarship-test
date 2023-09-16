#ifndef INTERNSHIP_TEST_SPARSE_MATRIX_H
#define INTERNSHIP_TEST_SPARSE_MATRIX_H

#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

class SparseMatrix {
   public:
    struct Element {
        int32_t index;
        double value;
    };

    explicit SparseMatrix() {}
    SparseMatrix(fs::path fname);

    void Print() const;

    SparseMatrix operator*(const SparseMatrix &m);
    SparseMatrix operator^(uint32_t p);

    std::vector<std::vector<Element>> data;
};

#endif  // INTERNSHIP_TEST_SPARSE_MATRIX_H
