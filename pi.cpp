#include <iostream>
#include <chrono>

#include "BigReal.hpp"


Math::BigReal getPi(int decimal_precision)
{
    size_t binary_precision = (10 * decimal_precision + 2) / 3 + 10;

    Math::BigReal pi(0LL, binary_precision);
    Math::BigReal n0(0LL, binary_precision);
    Math::BigReal n1(1LL, binary_precision);
    Math::BigReal n2(2LL, binary_precision);
    Math::BigReal n4(4LL, binary_precision);
    Math::BigReal n8(8LL, binary_precision);
    Math::BigReal n16(16LL, binary_precision);

    Math::BigReal a(1LL, binary_precision);
    Math::BigReal b(4LL, binary_precision);
    Math::BigReal c(5LL, binary_precision);
    Math::BigReal d(6LL, binary_precision);
    Math::BigReal pow16(1LL, binary_precision);

    for(int i = 0; i <= decimal_precision; i++)
    {
        pi += (n4 / a - n2 / b - n1 / c - n1 / d) / pow16;

        a += n8;
        b += n8;
        c += n8;
        d += n8;
        pow16 *= n16;
    }

    return pi;
}


int main(int argc, char *argv[])
{
    int decimal_precision{100};

    if(argc == 2)
    {
        try
        {
            decimal_precision = std::stoi(argv[1]);
        }
        catch(...)
        {
            std::cerr << "Conversion error\n";
            return EXIT_FAILURE;
        }
    }

    if(decimal_precision < 0)
    {
        std::cerr << "Negative precision\n";
        return EXIT_FAILURE;
    }


    auto start{ std::chrono::high_resolution_clock::now() };

    Math::BigReal pi = getPi(decimal_precision);

    auto end{ std::chrono::high_resolution_clock::now() };
    auto duration { std::chrono::duration_cast<std::chrono::milliseconds>(end - start) };

    std::cout << "The first " << decimal_precision << " digits after the decimal point of pi are:\n";
    std::cout << pi.toString(decimal_precision) << "\n\n";
    std::cout << "Calculation time: " << duration << '\n';

    return 0;
}
