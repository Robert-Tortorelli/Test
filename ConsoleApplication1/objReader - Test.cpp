//***
// This project's "objRenderer" files, including related files such as .hlsl and .obj files, are the latest versions and are intended to replace those in the project located at "repo\DirectX\objRenderer".
// This project's objRenderer.cpp file cannot be tested under this project, because it is a Windows app and this is a console project.
//
// The primary source code files located at "repo\DirectX\objRenderer" are temporarily READ-ONLY.
//***
//
// objReader - Test
// Version 1.0
//
// Description
// This program facilitates testing the objReader function in isolation.
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

// Using Declarations and Directives.
// Using declarations such as using std::string;   bring one identifier	 in the named namespace into scope.
// Using directives	  such as using namespace std; bring all identifiers in the named namespace into scope.
// Using declarations are preferred to using directives.
// Using declarations and directives must appear after their respective header file includes.
using std::cout;
using std::to_string;

// End: Global Declarations.

int main()
{
	cout << "Hello World!\n";

    int objReaderRC = objReader();							// Call a function and save its return code.
	if (objReaderRC == 1)									// Does the called function's return code indicate an error?
	{
		// The called function's return code indicates an error. Terminate this program with the called function's return code.
		cout << "Terminating. Function objReader failed with return code: " + to_string(objReaderRC);
		return objReaderRC;
	}

	// Display OurVertices, a variable containing values formatted for DirectX, in the Wavefront .obj file format.
	// OurVertices is the array of all object geometric vertices used to initialize the GPU's vertex buffer.
	int Vi = 0;
	while (Vi < GeometricVerticesTotal)
	{
		cout << "OurVertices[" + to_string(Vi) + "].X is "		  + to_string(OurVertices[Vi].X) + "\n";
		cout << "OurVertices[" + to_string(Vi) + "].Y is "		  + to_string(OurVertices[Vi].Y) + "\n";
		cout << "OurVertices[" + to_string(Vi) + "].Z is "		  + to_string(-OurVertices[Vi].Z) + "\n"; // Invert each DirectX formatted object geometric vertex's Z coordinate (i.e., -OurVertices[Vi].Z) to display it as specified on each object geometric vertex of the Wavefront .obj file using the format "v X, Y, Z".

		cout << "\n";
		++Vi;
	}

	// Display OurIndices, a variable containing values formatted for DirectX, in the Wavefront .obj file format.
	// OurIndices is the array of triangle primitive vertex indices used to initialize the GPU's index buffer.
	int Ii = 0;
	while (Ii < PrimitivesTotal * 3)
	{
		// Triangle primitive vertex indices formatted for DirectX are displayed in reverse order relative to the order in the Wavefront .obj file.
		cout << "OurIndices[" + to_string(Ii) + "] is " + to_string(OurIndices[Ii] + 1) + "\n";			  // Increment each DirectX formatted triangle primitive vertex index to display it as specified on each vertex index face element line of the Wavefront .obj file using the format "f v1 v2 v3".

		cout << "\n";
		++Ii;
	}

	cout << "GeometricVerticesTotal: " + to_string(GeometricVerticesTotal) + "\nPrimitivesTotal: " + to_string(PrimitivesTotal);

	// Terminate this program with a return code indicating success.
	return 0;
}