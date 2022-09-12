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
// OurVertices, a variable containing values formatted for DirectX, is the array of all geometric vertices used to initialize the GPU's vertex buffer.
// "Vn" is computed at run time to be the number of geometric vertices comprising the object, e.g., 8 geometric vertices comprise the 8 corners of a cube object, with each vertex specified by one geometric vertex line in the Wavefront .obj file using the format "v X, Y, Z".
// After "Vn" is computed, allocate OurVertices as an array (OurVertices[VertexNo]) of type VERTEX using "OurVertices = new VERTEX[Vn]".
extern int Vn;
extern VERTEX* OurVertices;									// A pointer to the first element in an array of VERTEX structures, with each element representing one geometric vertex.
//
// OurIndices, a variable containing values formatted for DirectX, is the array of all primitive (triangle) vertex indices, referencing the geometric vertices of OurVertices, used to initialize the GPU's index buffer.
// "In" is computed at run time to be the number of primitives comprising the object, e.g., The 12 faces (12 triangles) comprising a cube's 6 sides, with each face's (triangle's) 3 vertex indices specified by one vertex index face element line in the Wavefront .obj file using the format "f v1 v2 v3".
// The total number of triangle vertex indices is the number of triangles ("In") times the number of vertices in each triangle (3), i.e., (In * 3) or (12 * 3) equaling 36.
// In this example, each triangle vertex index (v1, v2, and v3 in "f v1 v2 v3") in OurIndices has a value of 0 - 7 (as per DirectX formatting) referencing one of the 8 geometric vertices (X, Y, and Z in "v X, Y, Z").
// After "In" is computed, allocate OurIndices as an array (OurIndices[FaceElementNo]) of type DWORD using OurIndices = new DWORD[In * 3].
extern int In;
extern DWORD* OurIndices;									// A pointer to the first element in an array of primitive vertex indices, with each element referencing one vertex of OurVertices.