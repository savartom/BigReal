#include "BigReal.hpp"


namespace Math
{

BigReal::BigReal(size_t precision)
    : digits {}
    , precision { static_cast<int>(precision) }
    , sign { false }
{
}


BigReal::BigReal(double number, size_t precision)
    : digits {}
    , precision { static_cast<int>(precision) }
{
    const union
    {
        double number;
        // The IEEE 754 standard
        struct
        {
            std::uint64_t fraction : 52;
            std::uint64_t exponent : 11;
            std::uint64_t sign : 1;
        };
    } cast = {number};

    this->sign = cast.sign;
    int shift = cast.exponent - 1023;

    if(shift == -1023)
    {
        this->sign = false;
    }
    else
    {
        std::uint64_t mantissa { (1ULL << 52) + cast.fraction };
        while(mantissa)
        {
            uint32_t digit { static_cast<uint32_t>(mantissa) };
            this->digits.push_back(digit);
            mantissa >>= 32;
        }
        int true_precision = 52 - shift;
        if(precision > true_precision)
        {
            *this <<= precision - true_precision;
        }
        else
        {
            this->precision = true_precision;
        }
    }
}


BigReal::BigReal(long long number, size_t precision)
    : digits {}
    , precision { static_cast<int>(precision) }
{
    this->sign = number < 0;
    number = std::abs(number);
    while(number)
    {
        uint32_t digit { static_cast<uint32_t>(number) };
        this->digits.push_back(digit);
        number >>= 32;
    }
    *this <<= precision;
}


BigReal::BigReal(const std::string& number, int base, size_t precision)
    : digits {}
    , precision { static_cast<int>(precision) }
{
    int point { static_cast<int>(number.size()) };
    const auto index { number.find('.') };
    if(index != std::string::npos)
    {
        point = static_cast<int>(index);
    }

    const BigReal bigBase { static_cast<long long>(base) };
    std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    BigReal beforePoint(precision);
    for(int i = 0; i < point; i++)
    {
        const char c = number[i];
        if(('0' <= c && c <= '9') || ('A' <= c && c <= 'Z'))
        {
            BigReal value(static_cast<long long>(alphabet.find(c)), precision);
            beforePoint = beforePoint * bigBase + value;
        }
    }

    BigReal afterPoint(precision);
    for(int i = static_cast<int>(number.size()) - 1; i > point; i--)
    {
        const char c = number[i];
        if(('0' <= c && c <= '9') || ('A' <= c && c <= 'Z'))
        {
            BigReal value(static_cast<long long>(alphabet.find(c)), precision);
            afterPoint = afterPoint / bigBase + value;
        }
    }
    afterPoint /= bigBase;

    *this = beforePoint + afterPoint;
    this->sign = (number.front() == '-' && !this->digits.empty());
}


bool operator==(const BigReal& first, const BigReal& second)
{
    if(first.sign != second.sign)
    {
        return false;
    }

    BigReal copy_first = first;
    BigReal copy_second = second;

    if(first.precision > second.precision)
    {
        copy_second.setPrecision(first.precision);
    }
    if(first.precision < second.precision)
    {
        copy_first.setPrecision(second.precision);
    }

    return copy_first.digits == copy_second.digits;
}


bool operator<(const BigReal& first, const BigReal& second)
{
    if(first.sign != second.sign)
    {
        return first.sign;
    }

    BigReal copy_first = first;
    BigReal copy_second = second;

    if(first.precision > second.precision)
    {
        copy_second.setPrecision(first.precision);
    }
    if(first.precision < second.precision)
    {
        copy_first.setPrecision(second.precision);
    }

    if(copy_first.digits.size() != copy_second.digits.size())
    {
        return first.sign ^ (copy_first.digits.size() < copy_second.digits.size());
    }

    return first.sign ^ std::lexicographical_compare(copy_first.digits.rbegin(), copy_first.digits.rend(),
                                                     copy_second.digits.rbegin(), copy_second.digits.rend());
}


bool operator!=(const BigReal& first, const BigReal& second)
{
    return !(first == second);
}


bool operator<=(const BigReal& first, const BigReal& second)
{
    return first < second || first == second;
}


bool operator>(const BigReal& first, const BigReal& second)
{
    return second < first;
}


bool operator>=(const BigReal& first, const BigReal& second)
{
    return second <= first;
}


BigReal BigReal::operator+() const
{
    return *this;
}


BigReal BigReal::operator++()
{
    return *this += 1LL;
}


BigReal BigReal::operator++(int)
{
    *this += 1LL;
    return *this - 1LL;
}


BigReal BigReal::operator-() const
{
    BigReal copy_number = *this;
    copy_number.flipSign();
    return copy_number;
}


BigReal BigReal::operator--()
{
    return *this -= 1LL;
}


BigReal BigReal::operator--(int)
{
    *this -= 1LL;
    return *this + 1LL;
}


BigReal operator+(const BigReal& first, const BigReal& second)
{
    BigReal result = first;
    result += second;
    return result;
}


BigReal operator-(const BigReal& first, const BigReal& second)
{
    BigReal result = first;
    result -= second;
    return result;
}


BigReal operator*(const BigReal& first, const BigReal& second)
{
    BigReal result = first;
    result *= second;
    return result;
}


BigReal operator/(const BigReal& first, const BigReal& second)
{
    BigReal result = first;
    result /= second;
    return result;
}


BigReal operator%(const BigReal& first, const BigReal& second)
{
    BigReal result = first;
    result %= second;
    return result;
}


BigReal& BigReal::operator+=(const BigReal& number)
{
    if(this->isZero())
    {
        return *this = number;
    }

    if(number.isZero())
    {
        return *this;
    }

    if(this->sign != number.sign)
    {
        this->flipSign();
        *this -= number;
        this->flipSign();
        return *this;
    }

    int precision = std::max(this->precision, number.precision);

    BigReal other = number;
    other.setPrecision(precision);
    this->setPrecision(precision);
    if(this->digits.size() < other.digits.size())
    {
        this->digits.resize(other.digits.size(), 0);
    }
    if(this->digits.size() > other.digits.size())
    {
        other.digits.resize(this->digits.size(), 0);
    }

    uint32_t digit {};
    for(int i = 0; i < this->digits.size(); i++)
    {
        uint64_t value { digit };
        value += this->digits[i];
        value += other.digits[i];

        this->digits[i] = value & 0xFFFFFFFF;
        digit = (value >> 32);
    }
    this->digits.push_back(digit);

    removeLeadingZeros();
    return *this;
}


BigReal& BigReal::operator-=(const BigReal& number)
{
    if(this->isZero())
    {
        *this = number;
        this->flipSign();
        return *this;
    }

    if(number.isZero())
    {
        return *this;
    }

    if(this->sign != number.sign)
    {
        this->flipSign();
        *this += number;
        this->flipSign();
        return *this;
    }

    if(this->abs() == number.abs())
    {
        this->digits.clear();
        this->sign = false;
        return *this;
    }

    int precision = std::max(this->precision, number.precision);

    BigReal other = number;
    other.setPrecision(precision);
    this->setPrecision(precision);
    if(this->digits.size() < other.digits.size())
    {
        this->digits.resize(other.digits.size(), 0);
    }
    if(this->digits.size() > other.digits.size())
    {
        other.digits.resize(this->digits.size(), 0);
    }

    uint32_t digit {};
    if(this->abs() > other.abs())
    {
        for(int i = 0; i < this->digits.size(); i++)
        {
            uint64_t value { this->digits[i] };
            value -= other.digits[i];
            value -= digit;

            this->digits[i] = value;
            digit = (value >> 32) ? 1 : 0;
        }
    }
    else
    {
        this->flipSign();
        for(int i = 0; i < this->digits.size(); i++)
        {
            uint64_t value { other.digits[i] };
            value -= this->digits[i];
            value -= digit;

            this->digits[i] = value;
            digit = (value >> 32) ? 1 : 0;
        }
    }

    removeLeadingZeros();
    return *this;
}


BigReal& BigReal::operator*=(const BigReal& number)
{
    if(this->isZero() || number.isZero())
    {
        this->digits.clear();
        this->sign = false;
        return *this;
    }

    std::vector<uint32_t> result(this->digits.size() + number.digits.size(), 0);

    for(int i = 0; i < this->digits.size(); i++)
    {
        if(this->digits[i] != 0)
        {
            uint32_t digit {};
            for(int j = 0; j < number.digits.size(); j++)
            {
                uint64_t value { digit +
                    static_cast<uint64_t>(this->digits[i])
                    * static_cast<uint64_t>(number.digits[j]) };
                value += result[i + j];
                result[i + j] = value;
                digit = value >> 32;
            }
            result[i + number.digits.size()] = digit;
        }
    }

    int new_precision = std::max(this->precision, number.precision);

    this->digits = result;
    this->sign = this->sign != number.sign;
    this->precision += number.precision;
    removeLeadingZeros();
    this->setPrecision(new_precision);

    return *this;
}


BigReal& BigReal::operator/=(const BigReal& number)
{
    if(this->isZero() || number.isZero())
    {
        return *this;
    }

    BigReal quotient {};
    int size = std::max(this->digits.size(), number.digits.size());
    quotient.digits.resize(size, 0);
    quotient.precision = std::max(this->precision, number.precision);

    for(int i = size - 1; i >= 0; i--)
    {
        uint32_t right = -1;
        uint32_t left = 0;

        quotient.digits[i] = right;
        if((quotient * number).abs() <= this->abs())
        {
            left = right;
        }

        while(right - left > 1)
        {
            uint32_t middle = left + (right - left) / 2;
            quotient.digits[i] = middle;
            if((quotient * number).abs() > this->abs())
            {
                right = middle;
            }
            else
            {
                left = middle;
            }
        }

        quotient.digits[i] = left;
    }

    quotient.sign = this->sign != number.sign;
    quotient.removeLeadingZeros();

    BigReal remainder { *this - quotient * number };
    if(remainder.sign)
    {
        if(!number.sign)
        {
            quotient--;
        }
        else
        {
            quotient++;
        }
    }

    *this = quotient;
    return *this;
}


BigReal& BigReal::operator%=(const BigReal& number)
{
    *this = *this - *this / number * number;
    return *this;
}


BigReal operator<<(const BigReal& number, int64_t shift)
{
    BigReal result = number;
    result <<= shift;
    return result;
}


BigReal operator>>(const BigReal& number, int64_t shift)
{
    BigReal result = number;
    result >>= shift;
    return result;
}


BigReal& BigReal::operator<<=(int64_t shift)
{
    if(shift < 0)
    {
        return *this >>= -shift;
    }
    if(this->isZero())
    {
        return *this;
    }

    int64_t count_digits { shift / 32 };
    digits.insert(digits.begin(), count_digits, 0);

    shift %= 32;
    if(shift == 0)
    {
        return *this;
    }

    uint32_t digit {};
    for(int i = 0; i < this->digits.size(); i++)
    {
        uint32_t value { (this->digits[i] << shift) + digit };
        digit = this->digits[i] >> (32 - shift);
        this->digits[i] = value;
    }
    this->digits.push_back(digit);

    removeLeadingZeros();
    return *this;
}


BigReal& BigReal::operator>>=(int64_t shift)
{
    if(shift < 0)
    {
        return *this <<= -shift;
    }
    if(this->isZero())
    {
        return *this;
    }

    int64_t count_digits { shift / 32 };
    digits.erase(digits.begin(), digits.begin() + count_digits);

    shift %= 32;
    if(shift == 0)
    {
        return *this;
    }

    uint32_t digit {};
    for(int i = this->digits.size() - 1; i >= 0; i--)
    {
        uint32_t value { (this->digits[i] >> shift) + digit };
        digit = this->digits[i] << (32 - shift);
        this->digits[i] = value;
    }

    removeLeadingZeros();
    return *this;
}


std::string BigReal::toString(uint64_t precision) const
{
    BigReal number { this->abs() };
    for(int i = 0; i < precision; i++)
    {
        number *= 10LL;
    }
    number.setPrecision(0);

    std::string string;
    while(!number.isZero())
    {
        BigReal quotient = number / 10LL;
        BigReal remainder = number - quotient * 10LL;
        number = quotient;

        if(!remainder.digits.empty())
        {
            string += static_cast<char>(remainder.digits.back() + '0');
        }
        else
        {
            string += '0';
        }

        if(string.size() == precision)
        {
            string += '.';
        }
    }

    if(string.size() < precision)
    {
        while(string.size() < precision)
        {
            string += '0';
        }
        string += '.';
    }

    if(string.back() == '.' || string.empty())
    {
        string += '0';
    }

    if(this->sign)
    {
        string += '-';
    }

    std::reverse(string.begin(), string.end());

    return string;
}


int BigReal::getPrecision() const
{
    return this->precision;
}


void BigReal::setPrecision(int precision)
{
    if(precision == this->precision)
    {
        return;
    }

    if(precision > this->precision)
    {
        *this <<= (precision - this->precision);
    }
    else
    {
        *this >>= (this->precision - precision);
    }

    this->precision = precision;
    removeLeadingZeros();
}


void BigReal::flipSign()
{
    this->sign = !this->sign;
}


bool BigReal::isZero() const
{
    return this->digits.empty();
}


void BigReal::removeLeadingZeros()
{
    while(!this->digits.empty() && this->digits.back() == 0)
    {
        this->digits.pop_back();
    }
    if(this->digits.empty())
    {
        this->sign = false;
    }
}


BigReal BigReal::abs() const
{
    return *this >= 0LL ? *this : -(*this);
}


std::ostream& operator<<(std::ostream& ostream, const BigReal& number)
{
    ostream << number.toString(number.precision);
    return ostream;
}

}


namespace literals
{

    Math::BigReal operator""_longnum(long double number)
    {
        return Math::BigReal { static_cast<double>(number) };
    }


    Math::BigReal operator""_longnum(unsigned long long number)
    {
        return Math::BigReal { static_cast<long long>(number) };
    }
}
