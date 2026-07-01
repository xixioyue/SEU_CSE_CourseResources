//P223：6（f、h）
#include <bits/stdc++.h>
using namespace std;

struct Triple {
    int row, col;
    long long value;
};

class Matrix {
public:
    int rows, cols;
    
    vector<unordered_map<int,long long>> row_data;

    Matrix(): rows(0), cols(0) {}
    Matrix(int r, int c): rows(r), cols(c), row_data(r) {}

    void set(int r, int c, long long v) {
        if (v == 0) {
            auto it = row_data[r].find(c);
            if (it != row_data[r].end()) row_data[r].erase(it);
        } else {
            row_data[r][c] = v;
        }
    }

    long long get(int r, int c) const {
        auto it = row_data[r].find(c);
        if (it == row_data[r].end()) return 0;
        return it->second;
    }

    size_t nonzeros() const {
        size_t cnt = 0;
        for (int i = 0; i < rows; ++i) cnt += row_data[i].size();
        return cnt;
    }

    //f
    Matrix operator+(const Matrix& b) const {
        if (rows != b.rows || cols != b.cols) {
            throw invalid_argument("Matrix dimensions must agree for addition.");
        }
        Matrix res(rows, cols);
        
        for (int i = 0; i < rows; ++i) {
            for (auto &p : row_data[i]) {
                res.row_data[i][p.first] = p.second;
            }
        }
        
        for (int i = 0; i < b.rows; ++i) {
            for (auto &p : b.row_data[i]) {
                int c = p.first;
                long long bv = p.second;
                long long av = res.get(i, c);
                long long sum = av + bv;
                if (sum == 0) {
                    auto it = res.row_data[i].find(c);
                    if (it != res.row_data[i].end()) res.row_data[i].erase(it);
                } else {
                    res.row_data[i][c] = sum;
                }
            }
        }
        return res;
    }

    //h
    Matrix operator*(const Matrix& b) const {
        if (cols != b.rows) {
            throw invalid_argument("Matrix dimensions must agree for multiplication (A.cols == B.rows).");
        }
        Matrix res(rows, b.cols);
        
        for (int i = 0; i < rows; ++i) {
            unordered_map<int,long long> acc;
            for (const auto &ak : row_data[i]) {
                int k = ak.first;
                long long a_ik = ak.second;
                if (k < 0 || k >= b.rows) continue;

                const auto &brow = b.row_data[k];
                for (const auto &bj : brow) {
                    int j = bj.first;
                    long long b_kj = bj.second;
                    acc[j] += a_ik * b_kj;
                }
            }

            for (auto &p : acc) {
                if (p.second != 0) res.row_data[i][p.first] = p.second;
            }
        }
        return res;
    }

    friend istream& operator>>(istream& is, Matrix& m) {
        int r, c, nnz;
        if (!(is >> r >> c >> nnz)) return is;
        m = Matrix(r, c);
        for (int t = 0; t < nnz; ++t) {
            int ri, ci;
            long long v;
            is >> ri >> ci >> v;

            if (ri < 1 || ci < 1 || ri > r || ci > c) {
                continue;
            }
            m.set(ri-1, ci-1, v);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m) {
        size_t nnz = m.nonzeros();
        os << m.rows << " " << m.cols << " " << nnz << "\n";
        for (int i = 0; i < m.rows; ++i) {
            for (const auto &p : m.row_data[i]) {
                os << (i+1) << " " << (p.first+1) << " " << p.second << "\n";
            }
        }
        return os;
    }
};


int main() {
    return 0;
}