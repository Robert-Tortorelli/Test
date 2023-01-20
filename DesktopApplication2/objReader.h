// Wavefront .obj file I/O (objReader function) Header File
// Version 3.1
//
// Description
// This header file declares external global variables.
// Include it in all source files that reference these external global variables, and always in the objReader function source file that defines them.
// Then define these external global variables, and optionally initialize them, in one and only one source file, in this case the objReader function source file.
//
// Header files should not contain "using directives" (such as "using namespace std") or "using declarations" (such as "using std::cout").
//
// Authorship
// Robert John Tortorelli

//***
// Global Declarations.
//***

// Pragma Directives.
// Specify that the compiler include this header file only once when compiling source code files.
#pragma once												// Automatically included by Visual Studio 2022 when creating a header file.

// DWORD Header File.
#include <intsafe.h>										// Required for the DWORD data type. Note the main objRenderer program (with the WinMain function) includes the larger windows.h, containing IntSafe.h, instead.

// Global Function Declarations: Function prototypes for functions called by programs that include this header file. They are optional in the functions named in the function prototypes.
int objReader(void);

//***
// DirectX Global Declarations.
//***

// DirectXMath Header File.									// The DirectXMath API provides SIMD-friendly C++ types and functions for common linear algebra and graphics math operations common to DirectX programs.
#include <directxmath.h>                                    // Used with the namespace DirectX.

// Declare the VERTEX 'named structure' data type (a dynamic array of structures) with a member list representing the vertex elements of one vertex of the object.
// The input-layout object describes the VERTEX structure.
// The input element description structure defines the input-layout object.
//***
// If the VERTEX structure is changed then the input element description structure (defined in objRenderer.cpp) must be changed accordingly.

struct VERTEX
{
	float X, Y, Z;											// Geometric vertex.
	DirectX::XMFLOAT3 VertexNormal;							// Vertex normal vector.
	float U, V;												// Vertex texture coordinate.
};

// End: DirectX Global Declarations.

//***
// External Global Variables.
//***

// Declare global variables external by declaring them 'extern' here. Otherwise building the project results in Visual Studio Linker Tools Error LNK2005 (symbol already defined in object).
// The objReader function parses a single 3D object's descriptive information from a Wavefront .obj file and uses it to populate the variables OurVertices and OurIndices, which are used when rendering that object.
//
// OurVertices, a variable containing values formatted for DirectX, is the array of vertex elements for all vertices. It is used to initialize the GPU's vertex buffer.
// GeometricVerticesTotal is computed at run time to be the total number of geometric vertices comprising the object, e.g., the number of 'v' lines in the Wavefront .obj file defining a geometric vertex. 8 geometric vertices comprise the 8 corners of a cube object, each specified by one geometric vertex 'v' line (these could be repeated in an inefficient Wavefront .obj file).
// After GeometricVerticesTotal is computed, OurVertices is allocated as an array (OurVertices[VertexNo]) of type VERTEX using OurVertices = new VERTEX[GeometricVerticesTotal].
extern VERTEX* OurVertices;									// A pointer to the first element in an array of VERTEX structures, with each element representing one geometric vertex.
extern int GeometricVerticesTotal;
//
// OurIndices, a variable containing values formatted for DirectX, is the array of all primitive vertex indices, with each vertex index referencing one geometric vertex in OurVertices. It is used to initialize the GPU's index buffer.
// Each primitive's vertex indices are specified in the Wavefront .obj file as one vertex index face element line starting with the letter f and followed by (v1 v2 v3 ... vi) vertex indices, e.g., three vertex indices for a triangle primitive.
// PrimitivesTotal is computed at run time to be the total number of face elements comprising the object, e.g., the number of 'f' lines in the Wavefront .obj file defining a face element.  12 triangle primitives comprise a cube's 6 sides, each specified by one face element 'f' line.
// The total number of primitive vertex indices is the total number of primitives (PrimitivesTotal) times the number of vertices in each primitive, e.g., each of the 12 triangle primitives has 3 vertices, therefore (PrimitivesTotal * 3) = (12 * 3) = 36 primitive vertex indices.
// In this example, each primitive vertex index has a value of 0 - 7 (as per DirectX formatting) referencing one of the 8 geometric vertices in OurVertices.
// After PrimitivesTotal is computed, OurIndices is allocated as an array (OurIndices[FaceElementNo]) of type DWORD using OurIndices = new DWORD[PrimitivesTotal * 3].
extern DWORD* OurIndices;									// A pointer to the first element in an array of primitive vertex indices, with each element referencing one vertex of OurVertices.
extern int PrimitivesTotal;

// End: External Global Variables.

// End: Global Declarations.