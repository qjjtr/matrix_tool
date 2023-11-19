#include "poly.h"

#include <map>
#include <vector>

namespace {
    int64_t BinaryPow(int64_t x, uint64_t power) {
        if (power == 0) {
            return 1;
        }
        int64_t half = BinaryPow(x, power / 2);
        return half * half * (power % 2 == 0 ? 1ll : x);
    }
}

Poly::Poly(std::string_view str) {
    std::vector<std::string_view> tokens;
    size_t id = 0;
    while (id != std::string::npos) {
        size_t next_id = std::min(str.find('+', id + 1), str.find('-', id + 1));
        if (id != 0) {
            ++id;
        }
        tokens.push_back(next_id == std::string::npos ? str.substr(id) : str.substr(id, next_id));
        id = next_id;
    }

    for (auto token : tokens) {
        int64_t coef_up = 0;
        int64_t coef_down = 1;
        uint64_t power = 0;
        size_t xId = std::min(token.find('x'), token.size());
        if (0 == xId) {
            coef_up = 1;
        } else {
            bool is_up = true;
            bool negative = false;
            for (size_t id = 0; id < xId; ++id) {
                if (token[id] == '/') {
                    is_up = false;
                    continue;
                }
                if (token[id] == '-') {
                    negative = !negative;
                    continue;
                }
                if (token[id] == '+') {
                    continue;
                }
                if (is_up) {
                    coef_up = coef_up * 10 + token[id] - '0';
                } else {
                    coef_down = coef_down * 10 + token[id] - '0';
                }
            }
            if (negative) {
                coef_up *= -1;
            }
        }
        if (xId + 1 == token.size()) {
            power = 1;
        }
        for (size_t id = xId + 2; id < token.size(); ++id) {
            power = power * 10 + token[id] - '0';
        }
        if (coef_up != 0) {
            coefficients_[power] += Fraction(coef_up, coef_down);
        }
    }
}

Poly::Poly(const std::initializer_list<Fraction>& coefficients) {
    uint64_t i = 0;
    for (const auto coefficient : coefficients) {
        if (coefficient != 0) {
            coefficients_[i] = coefficient;
        }
        ++i;
    }
}

Poly::Poly(const std::initializer_list<std::pair<uint64_t, Fraction>>& coefficients) {
    for (const auto& [i, coefficient] : coefficients) {
        if (coefficient != 0) {
            coefficients_[i] = coefficient;
        }
    }
}

bool Poly::operator==(const Poly& other) const {
    return coefficients_ == other.coefficients_;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}

Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [i, coefficient] : other.coefficients_) {
        coefficients_[i] += coefficient;
        if (coefficients_[i] == 0) {
            coefficients_.erase(i);
        }
    }
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    return *this += -other;
}

Poly& Poly::operator*=(const Poly& other) {
    std::unordered_map<uint64_t, Fraction> new_coefficients;
    for (const auto& [i, lhs_coefficient] : coefficients_) {
        for (const auto& [j, rhs_coefficient] : other.coefficients_) {
            new_coefficients[i + j] += lhs_coefficient * rhs_coefficient;
        }
    }
    coefficients_ = new_coefficients;
    return *this;
}

Poly& Poly::operator/=(const Poly& other) {
    const static auto isNumber = [] (const Poly& poly) {
        return poly.coefficients_.empty() || poly.coefficients_.size() == 1 && poly.coefficients_.count(0);
    };
    if (!isNumber(*this) || !isNumber(other)) {
        // only for matrix
        throw "only numbers please";
    }
    coefficients_[0] /= other.coefficients_.at(0);
    return *this;
}

Poly Poly::operator-() const {
    Poly result = *this;
    for (const auto& [i, coefficient] : coefficients_) {
        result.coefficients_[i] = -coefficient;
    }
    return result;
}

Fraction Poly::operator()(int64_t x) const {
    Fraction result = 0;
    for (const auto& [i, coefficient] : coefficients_) {
        result += coefficient * BinaryPow(x, i);
    }
    return result;
}

std::string Poly::AsString() const {
    if (coefficients_.empty()) {
        return "0";
    }
    std::string result = "";
    std::map<uint64_t, Fraction> sorted_coefficients;
    for (const auto& pair : coefficients_) {
        sorted_coefficients.emplace(pair);
    }
    bool is_first = true;
    for (auto it = sorted_coefficients.rbegin(); it != sorted_coefficients.rend(); ++it) {
        if (is_first) {
            if (it->second < 0) {
                result += '-';
            }
        } else {
            result += it->second > 0 ? " + " : " - ";
        }
        Fraction positive = it->second > 0 ? it->second : -it->second;
        result += positive.AsString();
        if (it->first > 0) {
            result += "x^" + std::to_string(it->first);
        }
        is_first = false;
    }
    return result;
}

Poly operator+(const Poly& lhs, const Poly& rhs) {
    Poly result = lhs;
    result += rhs;
    return result;
}

Poly operator-(const Poly& lhs, const Poly& rhs) {
    Poly result = lhs;
    result -= rhs;
    return result;
}

Poly operator*(const Poly& lhs, const Poly& rhs) {
    Poly result = lhs;
    result *= rhs;
    return result;
}

Poly operator/(const Poly& lhs, const Poly& rhs) {
    Poly result = lhs;
    result /= rhs;
    return result;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    return os << poly.AsString();
}

std::istream& operator>>(std::istream& is, Poly& poly) {
    std::string str;
    is >> str;
    poly = Poly(str);
    return is;
}
