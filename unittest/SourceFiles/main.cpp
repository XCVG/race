#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Vector3.h"
#include <iostream>
unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}
TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}
TEST_CASE("Vector class operators", "[vector]")
{
    GIVEN("A Vector3")
    {
        Vector3 *v = new Vector3(0,0,0);
        REQUIRE(v->magnitude() == 0);
    }
}