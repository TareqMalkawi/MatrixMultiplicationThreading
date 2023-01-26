
// Tareq Malkawi //

// All rights reserved //

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>

int numOfThreads;

int numOfOdd;
int numOfEven;
int totalCells;

// used to create a crtical section when incrementing (numOfOdd,numOfEven,totalCells)
// and when writing the result of each cell to the result matrix
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// used to create a crtical section when writing on the output file
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int** firstMatrix = nullptr;
int** secondMatrix = nullptr;
int** resultMatrix = nullptr;

int matrixSize;

std::fstream file;

// read the "in.txt" file , parse the numbers to int and fill the matrices
void FillMatrices()
{
    std::vector<std::string> lines;

    file.open("in.txt", std::ios::in);

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }

    matrixSize = stoi(lines.at(0).substr(0, lines.at(0).find(" ", 1)));

    firstMatrix = new int* [matrixSize];
    secondMatrix = new int* [matrixSize];

    for (int i = 0; i < matrixSize; i++)
    {
        firstMatrix[i] = new int[matrixSize];
        secondMatrix[i] = new int[matrixSize];
    }

    int j = 0;
    const int matrixPartition = (lines.size() - 1) / 2; // the bounderies of the first matrix

    for (int i = 1; i <= matrixPartition; i++)
    {
        int pos = 0;
        for (int k = 0; k < matrixSize; k++)
        {
            firstMatrix[j][k] = stoi(lines.at(i).substr(pos, lines.at(i).find(" ", 1)));
            pos += 2;
            if (pos > (matrixSize * 2 - 2))
                break;
        }
        j++;
    }

    j = 0;

    for (int i = matrixPartition + 1; i < lines.size() - 1; i++)
    {
        int pos = 0;
        for (int k = 0; k < matrixSize; k++)
        {
            secondMatrix[j][k] = stoi(lines.at(i).substr(pos, lines.at(i).find(" ", 1)));
            pos += 2;
            if (pos > (matrixSize * 2 - 2))
                break;
        }
        j++;
    }
}

// thread function
void* MultiplyMatrices(void* threadId)
{
    /*
        assuming T <= N
        calculating the number of operations and the number of any remaining operations if exists
        complete row multiplication means 1 operation , so if we have for an instance 4x4 matrix
        and 3 threads (3 <= 4) , numOfOpertaions = 4 /3 = 1  and  numOfRestOperations = 4 % 3 = 1
        each thread will do complete row multiplication , however we still have 1 operation so one of them
        has to do more work , for example the first thread
    */
    int numOfRows = matrixSize;
    int numOfOpertaions = numOfRows / numOfThreads;
    int numOfRestOperations = numOfRows % numOfThreads;

    int startOperations = 0;
    int endOperations = 0;

    int t = (intptr_t)threadId;

    if (t == 0) // first thread has more work to do than others
    {
        startOperations = numOfOpertaions * t;
        endOperations = (numOfOpertaions * (t + 1)) + numOfRestOperations;
    }
    else
    {
        startOperations = numOfOpertaions * t + numOfRestOperations;
        endOperations = (numOfOpertaions * (t + 1)) + numOfRestOperations;
    }

    // printf("ThreadID = %lu, startLoop = %d, endLoop = %d\n\n", (intptr_t)threadId, startOperations, endOperations);

    pthread_mutex_lock(&mutex1);
    try
    {
        file.open("out.txt", std::ofstream::app);

        if (file.is_open())
        {

            file << "ThreadID =  " << (intptr_t)threadId << ", startLoop =  "
                << startOperations << ", endLoop =  " << endOperations << std::endl
                << std::endl;
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "Exception opening/reading/closing file\n";
    }

    file.close();
    pthread_mutex_unlock(&mutex1);

    for (int i = startOperations; i < endOperations; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            int result = 0.0f;

            for (int k = 0; k < matrixSize; k++)
            {
                result += firstMatrix[i][k] * secondMatrix[k][j];
            }

            pthread_mutex_lock(&mutex);
            ++totalCells;

            if (result % 2 == 0)
            {
                ++numOfEven;
            }
            else
            {
                ++numOfOdd;
            }

            resultMatrix[i][j] = result;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    FillMatrices();

    resultMatrix = new int* [matrixSize];

    for (int i = 0; i < matrixSize; i++)
    {
        resultMatrix[i] = new int[matrixSize];
    }

    numOfThreads = argc;

    //printf("number of threads : %d\n", numOfThreads);

    pthread_t threads[numOfThreads];

    for (int t = 0; t < numOfThreads; t++)
    {
        if (pthread_create(&threads[t], NULL, MultiplyMatrices, (void*)(intptr_t)t))
        {
            printf("Thread Creation Failed %d\n", t);
        }
    }

    for (int t = 0; t < numOfThreads; t++)
    {
        pthread_join(threads[t], NULL);
    }

    // deallocate dynamically allocated memory
    if (firstMatrix != nullptr || secondMatrix != nullptr || resultMatrix != nullptr)
    {
        for (int i = 0; i < matrixSize; i++)
        {
            delete[] firstMatrix[i];
            delete[] secondMatrix[i];
            delete[] resultMatrix[i];
        }

        delete[] firstMatrix;
        delete[] secondMatrix;
        delete[] resultMatrix;
    }

    // printf("numOfEven = %d, numOfOdd = %d, totalCells = %d\n\n", numOfEven, numOfOdd, totalCells);

    try
    {
        file.open("out.txt", std::ofstream::app);

        if (file.is_open())
        {
            file << "numOfEven =  " << numOfEven << ", numOfOdd =  "
                << numOfOdd << ", totalCells =  " << totalCells << std::endl;
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "Exception opening/reading/closing file\n";
    }

    file.close();

    pthread_exit(NULL);

    return 0;
}
