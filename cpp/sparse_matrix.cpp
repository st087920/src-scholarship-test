#include "sparse_matrix.h"

SparseMatrix::SparseMatrix(fs::path fname) {
    std::ifstream stream(fname);
    int32_t n_rows;
    stream >> n_rows;
    data.resize(n_rows);

    for (int32_t i = 0; i < n_rows; i++) {
        int32_t n_cols;
        stream >> n_cols;
        data[i].reserve(n_cols);
        for (int32_t j = 0; j < n_cols; j++) {
            int32_t index;
            double value;
            stream >> index >> value;
            data[i].push_back({index, value});
        }
    }
}

void SparseMatrix::Print() const {
    int32_t n = data.size();
    printf("%d rows:\n", n);
    for (int32_t i = 0; i < n; i++) {
        int32_t element_index = 0;
        for (int32_t j = 0; j < n; j++) {
            if (element_index < data[i].size()) {
                if (data[i][element_index].index == j) {
                    printf("%.2f ", data[i][element_index].value);
                    element_index++;
                } else {
                    printf("%.2f ", 0.);
                }
            } else {
                printf("%.2f ", 0.);
            }
        }
        printf("\n");
    }
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix &m) {
    SparseMatrix result;
    // Here's your code
    // gathering column indexes: indexes of columns with at least one nonzero entry
    std::vector<int32_t> col_indexes;
    for (const auto &vec: this->data) {
        for (const auto &elem: vec) {
            bool added = false;
            for (auto iter = col_indexes.begin(); iter != col_indexes.end(); ++iter) {
                if (*iter == elem.index) {
                    added = true;
                    break;
                } else if (*iter > elem.index) {
                    added = true;
                    col_indexes.insert(iter, elem.index);
                    break;
                }
            }
            if (!added) {
                col_indexes.push_back({elem.index});
            }
        }
    }
    result.data.resize(m.data.size());
    for (int32_t j = 0; j < m.data.size(); ++j) {
        if (m.data[j].empty()) {
            continue;
        }
        for (auto i: col_indexes) {
            double res_val = 0.0;
            for (const auto &el_1: m.data[j]) {
                for (const auto &el_0: this->data[el_1.index]) {
                    if (el_0.index == i) {
                        res_val += el_0.value * el_1.value;
                    } else if (el_0.index > i) {
                        break;
                    }
                }
            }
            if (std::abs(res_val) > 1e-10) {
                result.data[j].push_back({i, res_val});
            }
        }
    }
    return result;
}

SparseMatrix SparseMatrix::operator^(uint32_t p) {
    SparseMatrix result;
    SparseMatrix temp = *this;
    printf("%d", p);
    while (p > 0) {
        if (p % 2 == 1) {
          if (result.data.empty()) {
               result = temp;
          } else {
                result = result * temp;
          }
        }
        p /= 2;
        temp = temp * temp;
    }
    return result;
}