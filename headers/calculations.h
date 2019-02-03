#include <iostream>
#include <queue>
#include <math.h>
#include <stack>
#include <algorithm>
#include "mkl.h"
#include "../headers/utils.h"

void calculateRef(std::string filePath);
void calculateImpl(std::string filePath);
void calculateEingenvalues(float *&diag, float *&offDiag, int size, float &abstol, float *&eigenVals, int &eigenValCount);
int calculateStrumSequence(float *&diag, float *&offDiag, int size, float *&strumSeq, float candidate);
std::vector<std::pair<float, float>> calculateGershgorinDisks(float *&diag, float *&offDiag, int size);
void calculateEingenvaluesPara(float *&diag, float *&offDiag, int size, float &abstol, float *&eigenValsFound, int &eigenValCount);