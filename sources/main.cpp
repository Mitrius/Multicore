#include "mkl.h"
#include "../headers/calculations.h"

int main(int _argc, char const *argv[])
{

    std::string filePath = argv[1];
    //calculateRef(filePath);
    calculateImpl(filePath);
    return 0;
}
