#pragma once

#include "poly.h"

#include <vector>

class Matrix {
public:
    struct MatrixException : public std::exception {
        explicit MatrixException(const std::string& what);
        const char* what() const noexcept override;

    private:
        const std::string what_;
    };

public:
    Matrix() = default;
    explicit Matrix(const size_t N);
    Matrix(const size_t N, const size_t M);
    explicit Matrix(std::vector<std::vector<Poly>> matrix);

    Matrix(const Matrix& other) = default;
    Matrix(Matrix&& other) = default;

    Matrix& operator=(const Matrix& other) = default;
    Matrix& operator=(Matrix&& other) = default;

    static Matrix UnitMatrix(const size_t N);

    Poly Determinant() const;
    Matrix Inverted() const;

    Matrix operator-() const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator*=(const Poly& coef);

    std::vector<std::vector<Poly>> GetData() const;

private:
    void CalculateDeterminant(size_t i, std::vector<bool>& toGo, Poly current, Poly& result) const;

private:
    std::vector<std::vector<Poly>> matrix_;

};

Matrix operator+(const Matrix& lhs, const Matrix& rhs);
Matrix operator-(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, const Poly& coef);
