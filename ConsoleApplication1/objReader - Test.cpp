//***
// This project's objReader.* files are the latest versions and will replace those in the project located at repo\DirectX\objRenderer.
// The primary source code files located at repo\DirectX\objRenderer are temporarily READ-ONLY.
//***
//
// objReader - Test
// Version 1
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
	cout << "Hello World!\n";

    objReader();

	// Display OurVertices, a variable containing values formatted for DirectX, in the Wavefront .obj file format.
	// OurVertices is the array of all object geometric vertices used to initialize the GPU's vertex buffer.
	int Vi = 0;
	while (Vi < Vn)
	{
		cout << "OurVertices[" + to_string(Vi) + "].X is "		  + to_string(OurVertices[Vi].X) + "\n";
		cout << "OurVertices[" + to_string(Vi) + "].Y is "		  + to_string(OurVertices[Vi].Y) + "\n";
		cout << "OurVertices[" + to_string(Vi) + "].Z is "		  + to_string(-OurVertices[Vi].Z) + "\n"; // Invert each DirectX formatted object geometric vertex's Z coordinate (i.e., -OurVertices[Vi].Z) to display it as specified on each object geometric vertex of the Wavefront .obj file using the format "v X, Y, Z".

		cout << "OurVertices[" + to_string(Vi) + "].Color[0] is " + to_string(OurVertices[Vi].Color[0]) + " from OurVertices element no. / 1 (" + to_string(Vi/1) + ") mod 2 (" + to_string((Vi/1) % 2) + ")\n";
		cout << "OurVertices[" + to_string(Vi) + "].Color[1] is " + to_string(OurVertices[Vi].Color[1]) + " from OurVertices element no. / 2 (" + to_string(Vi/2) + ") mod 2 (" + to_string((Vi/2) % 2) + ")\n";
		cout << "OurVertices[" + to_string(Vi) + "].Color[2] is " + to_string(OurVertices[Vi].Color[2]) + " from OurVertices element no. / 3 (" + to_string(Vi/3) + ") mod 2 (" + to_string((Vi/3) % 2) + ")\n";
		cout << "OurVertices[" + to_string(Vi) + "].Color[3] is " + to_string(OurVertices[Vi].Color[3]) + " from OurVertices element no. / 4 (" + to_string(Vi/4) + ") mod 2 (" + to_string((Vi/4) % 2) + ")\n";

		cout << "\n";
		++Vi;
	}

	// Display OurIndices, a variable containing values formatted for DirectX, in the Wavefront .obj file format.
	// OurIndices is the array of triangle vertex indices used to initialize the GPU's index buffer.
	int Ii = 0;
	while (Ii < In * 3)
	{
		// Triangle vertex indices formatted for DirectX are displayed in reverse order relative to the order in the Wavefront .obj file.
		cout << "OurIndices[" + to_string(Ii) + "] is " + to_string(OurIndices[Ii] + 1) + "\n";			  // Increment each DirectX formatted triangle vertex index to display it as specified on each vertex index face element line of the Wavefront .obj file using the format "f v1 v2 v3".

		cout << "\n";
		++Ii;
	}

	cout << "Vn: " + to_string(Vn) + "\nIn: " + to_string(In);
}