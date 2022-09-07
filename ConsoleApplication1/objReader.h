// Wavefront .obj file I/O (objReader function) Header File
// Version 3.1
//
// Description
// This header file declares external global variables.
// Include it in all source files that reference these external global variables, and always in the objReader function source file that defines them.
// Then define these external global variables, and optionally initialize them, in one and only one source file, in this case the objReader function source file.
//
// An objReader function prototype must be declared in any source file that calls the objReader function, but need not be declared in the objReader function itself. Therefore, an objReader function prototype is not included in this header file, because this header file is always included in the objReader function source file.

// Specify that the compiler include this header file only once, when compiling source code files.
#pragma once												// Automatically included by Visual Studio 2022 when creating a header file.

// DWORD Header File.
#include <IntSafe.h>										// Required for the DWORD data type. Note the main objRenderer program (with the WinMain function) includes the larger windows.h, containing IntSafe.h, instead.

// Declare the VERTEX structure with a member list representing the vertex elements of one vertex of an object.
struct VERTEX
{
	float X, Y, Z;
	float Color[4];
};

// Declare global variables external by declaring them 'extern' here. Otherwise building the project results in Linker Tools Error LNK2005 (symbol already defined in object).
// The objReader function parses a single 3D object's descriptive information from a Wavefront .obj file and uses it to populate variables, OurVertices and OurIndices, used when rendering that object.
//
// OurVertices is used to initialize the GPU's vertex buffer.
// "Vn" is computed at run time to be the number of vertices comprising an object.
// After "Vn" is computed, allocate OurVertices as an array (OurVertices[VertexNo]) of type VERTEX using "OurVertices = new VERTEX[Vn]".
extern int Vn;												// The number of vertices comprising the object.
extern VERTEX* OurVertices;									// A pointer to the first element in the array of VERTEX structures, with each structure representing the vertex elements of one vertex of an object.
//
// OurIndices is used to initialize the GPU's index buffer.
// "In" is computed at run time to be the number of triangle primitives comprising an object.
// After "In" is computed, allocate OurIndices as an array (OurIndices[FaceElementNo]) of type DWORD using OurIndices = new DWORD[In * 3]. Three vertex indices (each pointing to a vertex in OurVertices) describe each triangle, and "In" is the number of triangles comprising the object. Therefore In * 3.
extern int In;												// The number of primitives comprising the object. In this program, they are triangle primitives.
extern DWORD* OurIndices;									// A pointer to the first element in the array of vertex indices of an object.