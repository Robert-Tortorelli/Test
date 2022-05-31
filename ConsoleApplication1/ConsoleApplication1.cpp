// Visual Studio Console Application template.
// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// _CRT_SECURE_NO_WARNINGS is necessary to allow a successful build by suppressing the build error "error C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS."
// The GitHub CodeQL work flow should identify this error in this program's processString() function and produce the following security alert:
// "No space for zero terminator.
// This rule identifies calls to malloc that call strlen to determine the required buffer size, but do not allocate space for the zero terminator."
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

void processString(const char* input)
{
    char* buffer = (char*)malloc(strlen(input));

    strcpy(buffer, input);                 // This statement intentionally contains a security issue to test that the GitHub CodeQL workflow is working and produces the aforementioned security alert.

}                                                           // Test Change
                                                            // Test Change
                                                            // Test Change
                                                            // Test Change
                                                            // Test Change