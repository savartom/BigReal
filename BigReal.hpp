#ifndef BIGREAL_H
#define BIGREAL_H


#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>


namespace Math
{

class BigReal
{
public:
    BigReal(size_t precision = 0);
    BigReal(double number, size_t precision = 52);
    BigReal(long long number, size_t precision = 0);
    BigReal(const std::string& number, int base=10, size_t precision = 0);

    ~BigReal() = default;
    BigReal(const BigReal& number) = default;
    BigReal& operator=(const BigReal& number) = default;
    BigReal(BigReal&& number) = default;
    BigReal& operator=(BigReal&& number) = default;

    friend std::ostream& operator<<(std::ostream& ostream, const BigReal& number);

    friend bool operator==(const BigReal& first, const BigReal& second);
    friend bool operator!=(const BigReal& first, const BigReal& second);
    friend bool operator<(const BigReal& first, const BigReal& second);
    friend bool operator<=(const BigReal& first, const BigReal& second);
    friend bool operator>(const BigReal& first, const BigReal& second);
    friend bool operator>=(const BigReal& first, const BigReal& second);

    BigReal operator+() const;
    BigReal operator++();
    BigReal operator++(int);
    BigReal operator-() const;
    BigReal operator--();
    BigReal operator--(int);

    friend BigReal operator+(const BigReal& first, const BigReal& second);
    friend BigReal operator-(const BigReal& first, const BigReal& second);
    friend BigReal operator*(const BigReal& first, const BigReal& second);
    friend BigReal operator/(const BigReal& first, const BigReal& second);
    friend BigReal operator%(const BigReal& first, const BigReal& second);

    BigReal& operator+=(const BigReal& number);
    BigReal& operator-=(const BigReal& number);
    BigReal& operator*=(const BigReal& number);
    BigReal& operator/=(const BigReal& number);
    BigReal& operator%=(const BigReal& number);

    friend BigReal operator<<(const BigReal& number, int64_t shift);
    friend BigReal operator>>(const BigReal& number, int64_t shift);

    BigReal& operator<<=(int64_t shift);
    BigReal& operator>>=(int64_t shift);

    std::string toString(uint64_t precision) const;

    int getPrecision() const; 

    void setPrecision(int precision);

    void flipSign();

    BigReal abs() const;

private:
    std::vector<uint32_t> digits;
    int precision;
    bool sign;

    bool isZero() const;
    void removeLeadingZeros();
};

    std::ostream& operator<<(std::ostream& ostream, const BigReal& number);
}

namespace literals
{
    Math::BigReal operator""_longnum(long double number);
    Math::BigReal operator""_longnum(unsigned long long number);
}

#endif //BIGREAL_H
