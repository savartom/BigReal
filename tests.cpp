#include <iostream>
#include <iomanip>

#include "BigReal.hpp"


void check(bool result)
{
    if(result)
    {
        std::cout << "OK\n";
    }
    else
    {
        std::cout << "FAIL\n";
    }
}


void testDefaultConstructor()
{
    std::cout << std::left << std::setw(30) <<  "Default Constructor: ";
    Math::BigReal number {};
    check(number.toString(1) == "0.0");
}


void testLongLongConstructor()
{
    std::cout << std::left << std::setw(30) << "Long Long Constructor: ";
    Math::BigReal number1 {124354323LL};
    Math::BigReal number2 {-8774392348124LL, 2};
    bool result = (number1.toString(0) == "124354323") && (number1.toString(5) == "124354323.00000")
        && (number2.toString(2) == "-8774392348124.00") && (number2.toString(0) == "-8774392348124");
    check(result);
}


void testDoubleConstructor()
{
    std::cout << std::left << std::setw(30) << "Double Constructor: ";
    Math::BigReal number1 {-123.0};
    Math::BigReal number2 {654.5};
    bool result = (number1.toString(0) == "-123") && (number1.toString(2) == "-123.00")
        && (number2.toString(2) == "654.50") && (number2.toString(0) == "654");
    check(result);
}


void testStringConstructor()
{
    std::cout << std::left << std::setw(30) << "String Constructor: ";
    Math::BigReal number1 {"234587654567876543212345678898765234567895432"};
    Math::BigReal number2 {"-10100101", 2};
    Math::BigReal number3 {"B7DAF332C1.B12A", 16, 10};
    bool result = (number1.toString(0) == "234587654567876543212345678898765234567895432")
        && (number2.toString(0) == "-165") && (number3.toString(2) == "789652386497.69");
    check(result);
}


void testLiteral()
{
    using namespace literals;
    std::cout << std::left << std::setw(30) << "Literals: ";
    Math::BigReal number1 = 124354323_longnum;
    Math::BigReal number2 = 654.5_longnum;
    bool result = (number1.toString(0) == "124354323") && (number2.toString(3) == "654.500");
    check(result);
}


void testComparison()
{
    std::cout << std::left << std::setw(30) << "Comparison: ";
    Math::BigReal x { 12345.54 };
    Math::BigReal y { 12345LL };
    Math::BigReal z { 12344.9 };
    Math::BigReal t { -12345LL };
    Math::BigReal w {};
    Math::BigReal a { 123456LL, 10 };
    Math::BigReal b { 123456LL, 2 };
    bool result = (x > y) && (x == x) && (x != y) && (z < y) && (z <= x) && (z >= z) &&
        (t <= x) && (y > t) && (t != z) && (t < w) && (z >= w) && (a == b);
    check(result);
}


void testAddition()
{
    std::cout << std::left << std::setw(30) << "Addition: ";
    Math::BigReal num1 { "63467915736240243634507236402335734334534535345255552453425" };
    Math::BigReal num2 { "45874929635672324624395623497325694223454354626264542352453" };
    Math::BigReal num3 { "23423234235325345457564915464597814572465778435763497573465" };

    Math::BigReal num12 { "109342845371912568258902859899661428557988889971520094805878" };
    Math::BigReal num13 { "86891149971565589092072151866933548907000313781019050026890" };
    Math::BigReal num23 { "69298163870997670081960538961923508795920133062028039925918" };

    Math::BigReal x { "-874573458347457344657.63253975", 10, 50 };
    Math::BigReal y { "-12452893523423424762457.932849453523", 10, 50 };
    Math::BigReal z { "-13327466981770882107115.565389203523", 10, 50 };

    bool result = (num1 + num2 == num12) && (num1 + num3 == num13) && (num2 + num3 == num23)
        && ((x + y).toString(10) == z.toString(10)) && (x + 0LL == x) && (0LL + x == x);
    check(result);
}


void testSubtraction()
{
    std::cout << std::left << std::setw(30) << "Subtraction: ";
    Math::BigReal num1 { "57676776768765434567643456654345653456745676543234562345432" };
    Math::BigReal num2 { "45678765456743234567893456787654323456555464565456465452578" };
    Math::BigReal num3 { "83358982435825294762435284356294564734248356374457845834453" };

    Math::BigReal num12 { "11998011312022199999749999866691330000190211977778096892854" };
    Math::BigReal num13 { "-25682205667059860194791827701948911277502679831223283489021" };
    Math::BigReal num23 { "-37680216979082060194541827568640241277692891809001380381875" };

    Math::BigReal x { "-4937345792355792435.2337846238233525", 10, 50 };
    Math::BigReal y { "-7237877342645788723568.2834792734923", 10, 50 };
    Math::BigReal z { "7232939996853432931133.0496946496689475", 10, 50 };

    bool result = (num1 - num2 == num12) && (num1 - num3 == num13) && (num2 - num3 == num23)
        && ((x - y).toString(10) == z.toString(10)) && (0LL - x == -x) && (y - 0LL == y);
    check(result);
}


void testMultiplication()
{
    std::cout << std::left << std::setw(30) << "Multiplication: ";
    Math::BigReal num1 { "-10832752317857018578917085922" };
    Math::BigReal num2 { "-73647981646791635717951475625" };
    Math::BigReal num3 { "-27983445734345783487634452341" };

    Math::BigReal num12 { "797810343889773256010903799127567541422424822224813651250" };
    Math::BigReal num13 { "303137736640360385451304867702881652942662208422111043402" };
    Math::BigReal num23 { "2060924297857087949269456996324130231908881533809685688125" };

    Math::BigReal x { "23346723846873215.232476246223843", 10, 50 };
    Math::BigReal y { "32427265878245.234627342834764565", 10, 50 };
    Math::BigReal z { "757070421568526134422490843230.005754985852835403647", 10, 50 };

    bool result = (num1 * num2 == num12) && (num1 * num3 == num13) && (num2 * num3 == num23)
        && ((x * y).toString(10) == z.toString(10));
    check(result);
}


void testDivision()
{
    std::cout << std::left << std::setw(30) << "Division: ";
    Math::BigReal num1 { "63746298576789314672346782497846534574272566234625665363466236" };
    Math::BigReal num2 { "16244786745534435345435345345435" };
    Math::BigReal num3 { "2131234543543533" };

    Math::BigReal num12 { "3924108058501455731272262672159" };
    Math::BigReal num13 { "29910503642081766359703355175516170688145014470" };
    Math::BigReal num23 { "7622242608044803" };

    Math::BigReal x { "826578234242343318951.2374681326487713234897712334123412335", 10, 50 };
    Math::BigReal y { "23748237467.32849346576134856791456746798213647782136498234", 10, 50 };
    Math::BigReal z { "34805877083.69952757636156803755511646158993244171142578125", 10, 50 };

    bool result = (num1 / num2 == num12) && (num1 / num3 == num13) && (num2 / num3 == num23)
        && ((x / y).toString(30) == z.toString(30)) && (x / x == 1LL);
    check(result);
}


void testModulo()
{
    std::cout << std::left << std::setw(30) << "Modulo: ";
    Math::BigReal num1 { "21569367846781589673569326577586127356715632756932786523532" };
    Math::BigReal num2 { "73873457678342478878723533872353" };
    Math::BigReal num3 { "67643234565" };

    Math::BigReal num12 { "46564192740610394604214967995902" };
    Math::BigReal num13 { "3419789512" };
    Math::BigReal num23 { "7391597693" };

    bool result = (num1 % num2 == num12) && (num1 % num3 == num13) && (num2 % num3 == num23)
        && (num12 % num12 == 0LL);
    check(result);
}


void testShift()
{
    std::cout << std::left << std::setw(30) << "Shift: ";
    Math::BigReal num1 { "55645645654626262462626235" };
    Math::BigReal num2 { "3454265246426426436443545" };

    Math::BigReal num1left { "1867158033214252409216344543723520" };
    Math::BigReal num1right { "1581544371863" };
    Math::BigReal num2left { "115905908321178768864647252541440" };
    Math::BigReal num2right { "98176123129" };

    bool result = (num1 << 25 == num1left) && (num1 >> 45 == num1right)
        && (num2 << 25 == num2left)  && (num2 >> 45 == num2right);
    check(result);
}


int main()
{
    testDefaultConstructor();
    testLongLongConstructor();
    testDoubleConstructor();
    testStringConstructor();
    testLiteral();
    testComparison();
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testModulo();
    testShift();
}
