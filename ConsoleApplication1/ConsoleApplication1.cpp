// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

void processString(const char* input)
{
    char* buffer = (char*)malloc(strlen(input));

    strcpy(buffer, input);

}