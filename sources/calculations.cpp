#include "../headers/calculations.h"
#include "omp.h"

void calculateRef(std::string filePath)
{
    float abstol = 0.001;
    float *d;
    float *e;

    int eigenValsAmount;
    int foundBlocks;
    int size;

    loadCSVFile(filePath, d, e, size);
    std::cout << "file loaded\n";
    float *foundEigenVals = new float[size];
    int *iblock = new int[size];
    int *isplit = new int[size];

    clock_t start = clock();
    LAPACKE_sstebz('A', 'E', size, 0.5, 0.5, 0.5, 0.5, abstol, d, e, &eigenValsAmount, &foundBlocks, foundEigenVals, iblock, isplit);
    double total = (double)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "total time: " << total << "\n";

    delete d;
    delete e;
    delete foundEigenVals;
    delete iblock;
    delete isplit;
}

void calculateImpl(std::string filePath)
{
    float abstol = 0.001;
    float *d;
    float *e;
    int size;

    loadCSVFile(filePath, d, e, size);

    float *foundEigenVals = new float[size];
    int eigenValCount;

    std::cout << "file loaded\n";
    clock_t start = clock();
    calculateEingenvaluesPara(d, e, size, abstol, foundEigenVals, eigenValCount);
    double total = (double)(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "total time: " << total << "\n";
    std::cout << "total eigenvalues: " << eigenValCount << "\n";

    /*for (int i = 0; i < eigenValCount; i++)
    {
        std::cout << foundEigenVals[i] << '\n';
    }*/
}
int calculateStrumSequence(float *&diag, float *&offDiag, int size, float candidate)
{
    int count = 0;
    float strumSeq[2];

    strumSeq[0] = 1;
    strumSeq[1] = diag[0] - candidate;

    if (strumSeq[1] < 0)
    {
        count++;
    }

    for (int i = 1; i < size; i++)
    {
        float prev = strumSeq[1];
        if (strumSeq[1] == 0.0)
        {
            prev = std::numeric_limits<float>::epsilon();
        }
        float strumVal = (diag[i] - candidate) - (offDiag[i - 1] * offDiag[i - 1]) / prev;
        if (strumVal < 0.0)
            count++;
        strumSeq[0] = strumSeq[1];
        strumSeq[1] = strumVal;
    }

    return count;
}
bool compareTwoIntervals(const std::pair<float, float> &left, const std::pair<float, float> &right)
{
    return left.first < right.first;
}
std::vector<std::pair<float, float>> calculateGershgorinDisks(float *&diag, float *&offDiag, int size)
{
    std::vector<std::pair<float, float>> disks;

    for (int i = 0; i < size; i++)
    {
        float radius;

        if (i == 0)
            radius = offDiag[0];
        else if (i == size - 1)
            radius = offDiag[size - 2];
        else
            radius = offDiag[i - 1] + offDiag[i];

        disks.push_back(std::make_pair(diag[i] - radius - 0.001, diag[i] + radius + 0.001));
    }
    std::sort(disks.begin(), disks.end(), compareTwoIntervals);

    std::stack<std::pair<float, float>> stackerro;
    stackerro.push(disks.front());

    for (unsigned int i = 1; i < disks.size(); i++)
    {
        std::pair<float, float> top = stackerro.top();
        std::pair<float, float> next = disks[i];
        if (top.second < next.first)
            stackerro.push(next);
        else if (top.second < next.second)
        {
            top.second = next.second;
            stackerro.pop();
            stackerro.push(top);
        }
    }
    disks.clear();
    int stackSize = stackerro.size();
    for (unsigned int i = 0; i < stackSize; i++)
    {
        disks.push_back(stackerro.top());
        stackerro.pop();
    }
    return disks;
}
void calculateEingenvalues(float *&diag, float *&offDiag, int size, float &abstol, float *&eigenValsFound, int &eigenValCount)
{
    std::vector<std::pair<float, float>> disks = calculateGershgorinDisks(diag, offDiag, size);
    eigenValCount = -1;
    std::queue<std::pair<float, float>> ranges;
    int changes[3];
    float points[3];

    for (unsigned int elem = 0; elem < disks.size(); elem++)
    {
        ranges.push(disks[elem]);

        while (!ranges.empty())
        {
            std::pair<float, float> range = ranges.front();
            ranges.pop();

            points[0] = range.first;
            points[2] = range.second;

            points[1] = points[0] + (points[2] - points[0]) / 2;
            float tolerance = abs(points[2] - points[0]);

            if (tolerance < abstol)
            {
                eigenValCount++;
                eigenValsFound[eigenValCount] = points[1];
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    changes[i] = calculateStrumSequence(diag, offDiag, size, points[i]);
                }

                int eigenValsLeft = abs(changes[1] - changes[0]);
                int eigenValsRight = abs(changes[2] - changes[1]);

                if (eigenValsLeft)
                {
                    range.second = points[1];
                    ranges.push(range);
                }
                if (eigenValsRight)
                {
                    range.first = points[1];
                    range.second = points[2];
                    ranges.push(range);
                }
            }
        }
    }
}

void calculateEingenvaluesPara(float *&diag, float *&offDiag, int size, float &abstol, float *&eigenValsFound, int &eigenValCount)
{
    std::vector<std::pair<float, float>> disks = calculateGershgorinDisks(diag, offDiag, size);
    std::queue<std::pair<float, float>> ranges;
    eigenValCount = 0;
    float points[3];

    int changes[3];
    omp_set_nested(true);
#pragma omp parallel private(ranges, changes, points) shared(eigenValCount, eigenValsFound, disks)
    {

#pragma omp for
        for (unsigned int elem = 0; elem < disks.size(); elem++)
        {
            ranges.push(disks[elem]);

            while (!ranges.empty())
            {
                std::pair<float, float> range = ranges.front();
                ranges.pop();

                points[0] = range.first;
                points[2] = range.second;

                points[1] = points[0] + (points[2] - points[0]) / 2;
                float tolerance = abs(points[2] - points[0]);

                if (tolerance < abstol)
                {
#pragma omp critical
                    {
                        eigenValCount++;
                        eigenValsFound[eigenValCount] = points[1];
                    }
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        changes[i] = calculateStrumSequence(diag, offDiag, size, points[i]);
                    }

                    int eigenValsLeft = abs(changes[1] - changes[0]);
                    int eigenValsRight = abs(changes[2] - changes[1]);

                    if (eigenValsLeft)
                    {
                        range.second = points[1];
                        ranges.push(range);
                    }
                    if (eigenValsRight)
                    {
                        range.first = points[1];
                        range.second = points[2];
                        ranges.push(range);
                    }
                }
            }
        }
    }
}
