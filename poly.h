#pragma once

#include "fraction.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

class Poly {
public:
    Poly() = default;
    Poly(std::string_view  str);

    Poly(const Poly& other) = default;
    Poly(Poly&& other) = default;

    Poly& operator=(const Poly& other) = default;
    Poly& operator=(Poly&& other) = default;

    Poly(const std::initializer_list<Fraction>& coefficients);
    Poly(const std::initializer_list<std::pair<uint64_t, Fraction>>& coefficients);

    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;

    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);
    Poly& operator*=(const Poly& other);
    Poly& operator/=(const Poly& other);
    Poly operator-() const;

    Fraction operator()(int64_t x) const;

    std::string AsString() const;

private:
    std::unordered_map<uint64_t, Fraction> coefficients_;
};

Poly operator+(const Poly& lhs, const Poly& rhs);
Poly operator-(const Poly& lhs, const Poly& rhs);
Poly operator*(const Poly& lhs, const Poly& rhs);
Poly operator/(const Poly& lhs, const Poly& rhs);

std::ostream& operator<<(std::ostream& os, const Poly& poly);
std::istream& operator>>(std::istream& is, Poly& poly);
