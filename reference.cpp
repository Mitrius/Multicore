#include "mkl.h"
#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

void loadCSVFile(std::string &filePath, float *&d, float *&e, int &size)
{
    std::ifstream stream;
    stream.open(filePath);
    std::string temp;
    std::vector<std::string> tempStringContainer;

    if (stream.is_open())
    {
        std::getline(stream, temp);
        size = std::stoi(temp);

        d = new float[size];
        e = new float[size - 1];

        std::getline(stream, temp);
        boost::split(tempStringContainer, temp, boost::is_any_of(", "));
        for (int i = 0; i < size; i++)
        {
            d[i] = std::stof(tempStringContainer[i]);
        }
        tempStringContainer.clear();

        std::getline(stream, temp);
        boost::split(tempStringContainer, temp, boost::is_any_of(", "));
        for (int i = 0; i < size - 1; i++)
        {
            e[i] = std::stof(tempStringContainer[i]);
        }
    }
}

int main(int argc, char const *argv[])
{

    std::string filePath = argv[1];

    const float abstol = 0.001;
    float *d;
    float *e;

    int eigenValsAmount;
    int foundBlocks; //nsplit
    int size;

    loadCSVFile(filePath, d, e, size);

    float *foundEigenVals = new float[size];
    int *iblock = new int[size];
    int *isplit = new int[size];

    int info = LAPACKE_sstebz('A', 'E', size, 0.5, 0.5, 0.5, 0.5, abstol, d, e, &eigenValsAmount, &foundBlocks, foundEigenVals, iblock, isplit);

    for (int i = 0; i < eigenValsAmount; i++)
    {
        std::cout << foundEigenVals[i] << '\n';
    }
    delete d;
    delete e;
    delete foundEigenVals;
    delete iblock;
    delete isplit;

    return 0;
}
