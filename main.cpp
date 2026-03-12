#include <iostream>
#include "Matrix.h"
#include <complex>

template <typename T>
Matrix<T> inversed(const Matrix<T>& m) {
    Matrix<T> A1(m.get_rows(), m.get_columns());
    T det = m.determinant();

    for (size_t i = 0; i < m.get_rows(); i++) {
        for (size_t j = 0; j < m.get_rows(); j++) {
            Matrix<T> a(m.get_rows()-1, m.get_columns()-1);
            size_t x = 0;
            size_t y = 0;

            for (size_t k = 0; k < m.get_rows(); k++) {
                for (size_t l = 0; l < m.get_columns(); l++) {
                    if (k != i && l != j) {
                        a(y+1, x+1) = m(k+1, l+1);
                        if (x == a.get_rows() - 1) {
                            x = 0;
                            y++;
                        }
                        else
                            x++;
                    }
                }
            }
            if ((i + j) % 2 == 0) {
                A1(i + 1, j + 1) = a.determinant();
            }
            else {
                A1(i + 1, j + 1) = -a.determinant();
            }
        }
    }
    A1.transpose();
    Matrix<T> res = ((1/det) * A1);
    return res;
}

int main() {
    Matrix<double> m1(3, 3, 
        {1, 2, 0,
         1, 0, 3,
         1, 4, 0});
    Matrix<double> im1 = inversed(m1);
    std::cout << im1 * m1 << "\n";
}
