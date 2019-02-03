#include <regex>
#include <fstream>
#include <queue>
#include <stack>
#include <algorithm>

#include "math.h"
#include "../headers/utils.h"

void parseInputString(std::string &inputString, int size, float *&tab)
{
    std::regex reg(",");

    std::sregex_token_iterator iter(inputString.begin(), inputString.end(), reg, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> tempStringContainer(iter, end);
    for (int i = 0; i < size; i++)
    {
        tab[i] = std::stof(tempStringContainer[i]);
    }
}
void loadCSVFile(std::string &filePath, float *&d, float *&e, int &size)
{

    std::ifstream stream;
    stream.open(filePath);
    std::string temp;

    if (stream.is_open())
    {
        std::getline(stream, temp);
        size = std::stoi(temp);

        d = new float[size];
        e = new float[size];

        std::getline(stream, temp);
        parseInputString(temp, size, d);

        std::getline(stream, temp);
        parseInputString(temp, size - 1, e);
    }
}