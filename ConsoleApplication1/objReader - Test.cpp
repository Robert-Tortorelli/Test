//***
// This project's objReader.* files are the latest versions and will replace those in the project at repo\DirectX\objRenderer.
// The primary source code files at repo\DirectX\objRenderer are temporarily READ-ONLY.
//***
//
// objReader - Test
// Version 1
//
// Description
// This program facilitates unit testing the objReader function that parses a Wavefront.obj file.
//
// Authorship
// Robert John Tortorelli

// ***
// Global Declarations.
// ***

// Standard Encapsulated Data and Functions for Manipulating String Data.
#include <string>											// String class member functions stof, to_string, etc.

// Standard I/O Stream (Console Screen and Keyboard).
#include <iostream>											// Standard I/O Stream class member functions cin, cout, cerr, clog, etc.

// Wavefront .obj file I/O (objReader function) Header File.
#include "objReader.h"										// Declare external global variables in this header file, and include it in all source files that reference these external global variables.

// Each of the following using declarations brings one identifier, referenced in this source code file, into scope. These multiple individual using declarations are preferred to the single using directive using namespace std; as the latter brings everything in the std namespace into scope.
// Using declarations must appear after their respective header file includes.
using std::cout;
using std::string;
using std::to_string;

// Global Function Declarations: Function prototypes for functions called in this program.
void objReader(void);										// The objReader function is defined in file objReader.cpp.

// End: Global Declarations.

int main()
{
	string value;

	cout << "Hello World!\n";

    objReader();

	//***
	int Vi = 0;
	while (Vi <= Vn) //*HERE*
	{
		value = "OurVertices[0].X) is " + to_string(OurVertices[0].X);
		cout << value;
		++Vi;
	}
}