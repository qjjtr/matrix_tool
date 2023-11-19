#include "fraction.h"

#include <numeric>

Fraction::Fraction() : Fraction(0) {}

Fraction::Fraction(int64_t up, int64_t down)
    : up_(up)
    , down_(down)
{
    if (down_ == 0) {
        throw std::exception{};
    }
    Normalize();
}

Fraction::Fraction(int64_t n) : Fraction(n, 1) {}

Fraction Fraction::operator-() const {
    Fraction copy = *this;
    copy.up_ *= -1;
    return copy;
}

Fraction& Fraction::operator+=(const Fraction& other) {
    up_ = up_ * other.down_ + other.up_ * down_;
    down_ *= other.down_;
    Normalize();
    return *this;
}

Fraction& Fraction::operator-=(const Fraction& other) {
    return *this += -other;
}

Fraction& Fraction::operator*=(const Fraction& other) {
    up_ *= other.up_;
    down_ *= other.down_;
    Normalize();
    return *this;
}

Fraction& Fraction::operator/=(const Fraction& other) {
    up_ *= other.down_;
    down_ *= other.up_;
    Normalize();
    return *this;
}

void Fraction::Normalize() {
    auto gcd = std::gcd(up_, down_);
    up_ /= gcd;
    down_ /= gcd;
    if (down_ < 0) {
        up_ *= -1;
        down_ *= -1;
    }
}


bool Fraction::operator==(const Fraction& other) const {
    return up_ == other.up_ && down_ == other.down_;
}

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
}

bool Fraction::operator<(const Fraction& other) const {
    return up_ * other.down_ < other.up_ * down_;
}

bool Fraction::operator<=(const Fraction& other) const {
    return !(*this > other);
}

bool Fraction::operator>(const Fraction& other) const {
    return other < *this;
}

bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
}

std::string Fraction::AsString() const {
    std::string result = std::to_string(up_);
    if (down_ != 1) {
        result += '/' + std::to_string(down_);
    }
    return result;
}

Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
    Fraction result = lhs;
    result += rhs;
    return result;
}

Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
    Fraction result = lhs;
    result -= rhs;
    return result;
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
    Fraction result = lhs;
    result *= rhs;
    return result;
}

Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
    Fraction result = lhs;
    result /= rhs;
    return result;
}
