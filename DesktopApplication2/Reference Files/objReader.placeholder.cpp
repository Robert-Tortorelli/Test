// objReader
// Version 3.1
//
// Description
// This function is a test version that manually populates the global structures used by objRenderer with the values from DirectXTutorial.
//
// Authorship
// Robert John Tortorelli

//***
// Global Declarations.
//***

// Wavefront .obj file I/O (objReader function) Header File.
// Declare external global variables in this header file, and include it in all source files that reference these external global variables.
// Includes the DirectXMath Header File.
#include "objReader.h"

// Standard Encapsulated Data and Functions for Manipulating String Data.
#include <string>											// String class member functions stof, to_string, etc.

// File Stream.
#include <fstream>											// File stream class member functions get, close, etc.

// Using Declarations and Directives.
// Using declarations such as using std::string;   bring one identifier	 in the named namespace into scope.
// Using directives	  such as using namespace std; bring all identifiers in the named namespace into scope.
// Using declarations are preferred to using directives.
// Using declarations and directives must appear after their respective header file includes.
using std::fill;
using std::ifstream;
using std::ios;
using std::stof;
using std::string;

//***
// DirectX Global Declarations.
//***

// Using Declarations and Directives.
// Using declarations such as using std::string;   bring one identifier	 in the named namespace into scope.
// Using directives	  such as using namespace std; bring all identifiers in the named namespace into scope.
// Using declarations are preferred to using directives.
// Using declarations and directives must appear after their respective header file includes.
using namespace DirectX;                                    // Used with the DirectXMath Header File.

// End: DirectX Global Declarations.

//***
// External Global Variables.
//***

// Define external global variables in one and only one source file, this one, and initialize them as needed.
// See the associated header file for descriptions of these global variables.
VERTEX* OurVertices;
int GeometricVerticesTotal = 24;                            // Hard-coded, rather than computed, in this test version of objReader.
DWORD* OurIndices;
int PrimitivesTotal = 12;                                   // Hard-coded, rather than computed, in this test version of objReader.

// End: External Global Variables.

// End: Global Declarations.

// objReader function: Definition
int objReader(void)
{

	// Allocate and initialize the dynamic arrays of structures, i.e., the external (and therefore global) variables OurVertices and OurIndices.
	//
	// Allocate memory for OurVertices and OurIndices and initialize them. Empty brackets ("{}") initialize them to 0.0f values.

	// OurVertices
	// In the cube object specified below:
	// OurVertices has 24 array elements, composed of 4 sets of vertex attributes for each of the 6 sides of the cube.
	// e.g.,
	//   {-1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},   // side 1
	//   {v1,    v2,    v3,            vn1,  vn2,  vn3,   vt1,  vt2 },   // side 1 key: "vx" are geometric vertices; "vnx" are vertex normal vectors; "vtx" are vertex texture coordinates.
	OurVertices = new VERTEX[GeometricVerticesTotal] {
		{-1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},   // side 1
		{1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f},

		{-1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f}, // side 2
		{-1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f},
		{1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f},
		{1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f},

		{-1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f},   // side 3
		{-1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f},
		{1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f},

		{-1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f}, // side 4
		{1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f},
		{-1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f},
		{1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f},

		{1.0f, -1.0f, -1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f},   // side 5
		{1.0f, 1.0f, -1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
		{1.0f, -1.0f, 1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f},

		{-1.0f, -1.0f, -1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f}, // side 6
		{-1.0f, -1.0f, 1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
		{-1.0f, 1.0f, -1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
		{-1.0f, 1.0f, 1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f},
	};

	// The following examples show alternative initialization syntax, performed post-dynamic allocation. No variable Type can be specified post-dynamic allocation, as doing so would create a different new, and local, variable.
	// 1. Listing assigned values without referencing the structure's members:
	// OurVertices[0] = {-1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f)};
	//
	// 2. Listing assigned values by referencing the structure's members:
	//OurVertices[0].X = -1.0f;
	//OurVertices[0].Y = -1.0f;
	//OurVertices[0].Z = 1.0f;
	//OurVertices[0].VertexNormal.x = 0.0f;
	//OurVertices[0].VertexNormal.y = 0.0f;
	//OurVertices[0].VertexNormal.z = 1.0f;

	OurIndices = new DWORD[PrimitivesTotal * 3] {
		0, 1, 2,                                                        // side 1
		2, 1, 3,
		4, 5, 6,                                                        // side 2
		6, 5, 7,
		8, 9, 10,                                                       // side 3
		10, 9, 11,
		12, 13, 14,                                                     // side 4
		14, 13, 15,
		16, 17, 18,                                                     // side 5
		18, 17, 19,
		20, 21, 22,                                                     // side 6
		22, 21, 23,
	};

	// Terminate this function with a return code indicating success.
	return 0;
}