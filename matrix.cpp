#include "matrix.h"

Matrix::MatrixException::MatrixException(const std::string& what)
    : what_(what)
{}

const char* Matrix::MatrixException::what() const noexcept {
    return what_.c_str();
}

Matrix::Matrix(const size_t N)
    : Matrix(N, N)
{}

Matrix::Matrix(const size_t N, const size_t M)
    : matrix_(std::vector<std::vector<Poly>>(N, std::vector<Poly>(M, Poly{})))
{}

Matrix::Matrix(std::vector<std::vector<Poly>> matrix)
    : matrix_(matrix)
{}

Matrix Matrix::UnitMatrix(const size_t N) {
    Matrix unit(N);
    for (size_t i = 0; i < N; ++i) {
        unit.matrix_[i][i] = Poly{1};
    }
    return unit;
}

void Matrix::CalculateDeterminant(size_t line, std::vector<bool>& toGo, Poly current, Poly& result) const {
    if (line == toGo.size()) {
        result += current;
        return;
    }
    size_t id = 0;
    for (size_t i = 0; i < toGo.size(); ++i) {
        if (!toGo[i]) {
            continue;
        }
        toGo[i] = false;
        auto next = current * matrix_[line][i];
        if (id % 2 == 1) {
            next *= {-1};
        }
        CalculateDeterminant(line + 1, toGo, next, result);
        toGo[i] = true;
        ++id;
    }
}

Poly Matrix::Determinant() const {
    if (matrix_.size() != matrix_[0].size()) return {0};
    Poly result = {0};
    std::vector<bool> toGo(matrix_.size(), true);
    CalculateDeterminant(0, toGo, Poly{1}, result);
    return result;
}

Matrix Matrix::Inverted() const {
    if (matrix_.size() != matrix_[0].size()) {
        throw MatrixException("Try to invert non square matrix");
    }

    if (Determinant() == Poly{0}) {
        throw MatrixException("Try to invert degenerate matrix");
    }

    size_t N = matrix_.size();
    auto copy = *this;
    auto one = UnitMatrix(N);

    for (size_t line = 0; line < N; ++line) {
        size_t found = line;
        while (copy.matrix_[found][line] == Poly{0}) ++found;

        std::swap(copy.matrix_[line], copy.matrix_[found]);
        std::swap(one.matrix_[line], one.matrix_[found]);

        auto val = copy.matrix_[line][line];

        for (size_t j = 0; j < N; ++j) {
            copy.matrix_[line][j] /= val;
            one.matrix_[line][j] /= val;
        }

        for (size_t i = 0; i < N; ++i) {
            if (i == line) continue;
            auto coef = copy.matrix_[i][line];
            for (size_t j = 0; j < N; ++j) {
                copy.matrix_[i][j] -= copy.matrix_[line][j] * coef;
                one.matrix_[i][j] -= one.matrix_[line][j] * coef;
            }
        }
    }

    return one;
}

Matrix Matrix::operator-() const {
    return *this * Poly{-1};
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (matrix_.size() != other.matrix_.size()
        || matrix_[0].size() != other.matrix_[0].size())
    {
        throw MatrixException("Try to add matrixes of wrong sizes");
    }
    for (size_t i = 0; i < matrix_.size(); ++i) {
        for (size_t j = 0; j < matrix_[0].size(); ++j) {
            matrix_[i][j] += other.matrix_[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    return *this += -other;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (matrix_[0].size() != other.matrix_.size()) {
        throw MatrixException("Try to multiply matrixes of wrong sizes");
    }
    Matrix result(matrix_.size(), other.matrix_[0].size());
    for (size_t i = 0; i < matrix_.size(); ++i) {
        for (size_t j = 0; j < other.matrix_[0].size(); ++j) {
            for (size_t k = 0; k < matrix_[0].size(); ++k) {
                result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
            }
        }
    }
    std::swap(*this, result);
    return *this;
}

Matrix& Matrix::operator*=(const Poly& coef) {
    for (auto& line : matrix_) {
        for (auto& element : line) {
            element *= coef;
        }
    }
    return *this;
}

std::vector<std::vector<Poly>> Matrix::GetData() const {
    return matrix_;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;
    result += rhs;
    return result;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;
    result -= rhs;
    return result;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix result = lhs;
    result *= rhs;
    return result;
}

Matrix operator*(const Matrix& lhs, const Poly& coef) {
    Matrix result = lhs;
    result *= coef;
    return result;
}
