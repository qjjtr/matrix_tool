#pragma once

#include <cstdint>
#include <string>

class Fraction {
public:
    Fraction();
    Fraction(int64_t up, int64_t down);
    Fraction(int64_t n);

    Fraction(const Fraction& other) = default;
    Fraction(Fraction&& other) = default;

    Fraction& operator=(const Fraction& other) = default;
    Fraction& operator=(Fraction&& other) = default;

    Fraction operator-() const;
    Fraction& operator+=(const Fraction& other);
    Fraction& operator-=(const Fraction& other);
    Fraction& operator*=(const Fraction& other);
    Fraction& operator/=(const Fraction& other);

    bool operator==(const Fraction& other) const;
    bool operator!=(const Fraction& other) const;
    bool operator<(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;

    std::string AsString() const;

private:
    void Normalize();

private:
    int64_t up_;
    int64_t down_;
};

Fraction operator+(const Fraction& lhs, const Fraction& rhs);
Fraction operator-(const Fraction& lhs, const Fraction& rhs);
Fraction operator*(const Fraction& lhs, const Fraction& rhs);
Fraction operator/(const Fraction& lhs, const Fraction& rhs);
