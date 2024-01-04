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

// Declare the VERTEX 'named structure' data type (a dynamic array of structures) with members defining the attributes of one vertex of the associated object.
// The input-layout object describes the VERTEX structure.
// The input element description structure defines the input-layout object.
//***
// If the VERTEX structure is changed then the input element description structure (defined in objRenderer.cpp) must be changed accordingly.

struct VERTEX {
	float X, Y, Z;											// Geometric vertex.		  ("v"	element in the Wavefront .obj file)
	DirectX::XMFLOAT3 VertexNormal;							// Vertex normal vector.	  ("vn" element in the Wavefront .obj file)
	float U, V;												// Vertex texture coordinate. ("vt" element in the Wavefront .obj file)
};

// End: DirectX Global Declarations.

//***
// External Global Variables.
//***

// Declare global variables external by declaring them 'extern' here. Otherwise building the project results in Visual Studio Linker Tools Error LNK2005 (symbol already defined in object).
// The objReader function parses a single 3D object's descriptive information from a Wavefront .obj file and uses it to populate the variables OurVertices and OurIndices, which are used when rendering that object.
//
// OurVertices (used to initialize the GPU's vertex buffer), a variable containing values formatted for DirectX, is the array of all sets of vertex attributes.
// Each primitive's set of vertex attributes are specified in the Wavefront .obj file as multiple elements ("v", "vt", "vn").
extern VERTEX* OurVertices;									// A pointer to the first element in an array of VERTEX structures, with each array element representing one vertex and its attributes.
// After GeometricVerticesTotal is computed, OurVertices is allocated as an array (OurVertices[VertexNo]) of type VERTEX using OurVertices = new VERTEX[GeometricVerticesTotal].
extern int GeometricVerticesTotal;							// The total number of sets of vertex attributes.
//
// OurIndices (used to initialize the GPU's index buffer), a variable containing values formatted for DirectX, is an array of indices, with each index referencing an associated set of vertex attributes in OurVertices.
extern DWORD* OurIndices;									// A pointer to the first array element in an array of indices, with each array element referencing one set of vertex attributes in OurVertices.
// PrimitivesTotal is computed as the total number of face elements defining an object. A cube's 6 sides are comprised of 12 triangle primitives, each specified by one face element. Therefore PrimitivesTotal = 12.
// The total number of non-unique geometric vertex indices is the total number of primitives (PrimitivesTotal) times the number of geometric vertices in each primitive, e.g., each of the 12 triangle primitives has 3 geometric vertices, therefore (PrimitivesTotal * 3) = (12 * 3) = 36 vertex indices.
// After PrimitivesTotal is computed, OurIndices is allocated as an array (OurIndices[FaceElementNo]) of type DWORD using OurIndices = new DWORD[PrimitivesTotal * 3].
extern int PrimitivesTotal;									// The total number of face elements (triangle primitives) comprising an object.

// End: External Global Variables.

// End: Global Declarations.