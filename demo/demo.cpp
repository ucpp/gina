#include <iostream>
#include "gina.h"

using namespace gina;

int main()
{
    int32 result = Sum(2, 2);
    std::cout << "Hello, gina!" << std::endl;
    std::cout << "2 + 2 = " << result << std::endl;
}