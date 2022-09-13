// Wavefront .obj file I/O (objReader function) Header File
// Version 3.1
//
// Description
// This header file declares external global variables.
// Include it in all source files that reference these external global variables, and always in the objReader function source file that defines them.
// Then define these external global variables, and optionally initialize them, in one and only one source file, in this case the objReader function source file.
//
// An objReader function prototype must be declared in any source file that calls the objReader function, but not in the objReader function itself. Therefore, an objReader function prototype is not included in this header file, as doing so would unnecessarily place it in the objReader function.

// Specify that the compiler include this header file only once when compiling source code files.
#pragma once												// Automatically included by Visual Studio 2022 when creating a header file.

// DWORD Header File.
#include <IntSafe.h>										// Required for the DWORD data type. Note the main objRenderer program (with the WinMain function) includes the larger windows.h, containing IntSafe.h, instead.

// Declare the VERTEX structure with a member list representing the vertex elements of one vertex of the object.
struct VERTEX
{
	float X, Y, Z;
	float Color[4];
};

// Declare global variables external by declaring them 'extern' here. Otherwise building the project results in Visual Studio Linker Tools Error LNK2005 (symbol already defined in object).
// The objReader function parses a single 3D object's descriptive information from a Wavefront .obj file and uses it to populate the variables OurVertices and OurIndices, which are used when rendering that object.
//
// OurVertices, a variable containing values formatted for DirectX, is the array of all geometric vertices. It is used to initialize the GPU's vertex buffer.
// Each of the object's vertices is specified in the Wavefront .obj file as one geometric vertex line starting with the letter v and followed by (x,y,z[,w]) coordinates. W is optional and defaults to 1.0.
// "Vn" is computed at run time to be the number of geometric vertices comprising the object, e.g., 8 geometric vertices comprise the 8 corners of a cube object, each specified by one geometric vertex line.
// After "Vn" is computed, OurVertices is allocated as an array (OurVertices[VertexNo]) of type VERTEX using "OurVertices = new VERTEX[Vn]".
extern VERTEX* OurVertices;									// A pointer to the first element in an array of VERTEX structures, with each element representing one geometric vertex.
extern int Vn;
//
// OurIndices, a variable containing values formatted for DirectX, is the array of all primitive vertex indices, with each vertex index referencing one geometric vertex in OurVertices. It is used to initialize the GPU's index buffer.
// Each primitive's vertex indices are specified in the Wavefront .obj file as one vertex index face element line starting with the letter f and followed by (v1 v2 v3 ... vi) vertex indices, e.g., three vertex indices for a triangle primitive.
// "In" is computed at run time to be the number of primitives comprising the object, e.g., 12 triangle primitives comprise a cube's 6 sides, each specified by one vertex index face element line.
// The total number of primitive vertex indices is the number of primitives ("In") times the number of vertices in each primitive, e.g., each of the 12 triangle primitive has 3 vertices, therefore (In * 3) = (12 * 3) = 36 primitive vertex indices.
// In this example, each primitive vertex index has a value of 0 - 7 (as per DirectX formatting) referencing one of the 8 geometric vertices in OurVertices.
// After "In" is computed, OurIndices is allocated as an array (OurIndices[FaceElementNo]) of type DWORD using OurIndices = new DWORD[In * 3].
extern DWORD* OurIndices;									// A pointer to the first element in an array of primitive vertex indices, with each element referencing one vertex of OurVertices.
extern int In;