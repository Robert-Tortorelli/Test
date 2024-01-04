// objReader Pseudo Code
// Version 3.1
//
// Description
// This function parses a Wavefront .obj file for a single 3D object's descriptive information and assigns it to the external global variables used by the calling program when rendering the object.
//
// Authorship
// Robert John Tortorelli

//***
// Global Declarations.
// Declaring a pointer variable at the global scope (outside any function) automatically initializes it to nullptr if you don't provide an initial value.
// This ensures it does not point to any memory location until it is explicitly initialized.
//***

// Wavefront .obj file I/O Header File.
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
using std::vector;

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
// External Variable Global Definitions.
//***

//*HERE1* VertexAttributeSetsTotal and PrimitivesTotal, if kept and used, need to be assigned applicable values.
// Define external variables in one and only one source file (this one) and initialize them as needed.
// See the associated header file for declarations and descriptions of these external variables.
// OurVertices:
std::vector<VERTEX>* OurVertices;
int VertexAttributeSetsTotal = 0;
// OurIndices:
std::vector<DWORD>* OurIndices;
int PrimitivesTotal = 0;

// End: External Variable Global Definitions.

// End: Global Declarations.

//***
// Functions and local variables follow.
// Declaring a pointer variable at the local scope (inside a function) does not automatically initialize it to nullptr, and manually initializing it to nullptr or some initial safe value is recommended.
// This is because, while uninitialized, it doesn't point to a known location and could point anywhere, which is dangerous because you could inadvertently modify a random memory location.
//***

//*HERE2*
// objReader function: Definition
int objReader(void)
{
	//***
	// Local Declarations.
	//***

	ifstream obj;											// Declare the input file stream object representing the Wavefront .obj file.
	string stringtext;										// Holds one statement of the input file stream object representing the Wavefront .obj file.

	// Wavefront .obj file format requirements:
	// - Vertex attributes are listed first in a Wavefront .obj file, and will therefore be processed before any face elements, so that the face elements can refer to vertex attributes by number, using the order in which they occur in the file.
	// - No spaces are permitted before or after a slash ('/').
	// - Statements can start in any column.
	// - Statements can be logically joined with the line continuation character ( \ ) at the end of a line. (*This is not supported by this program*)
	//
	// objRenderer Wavefront .obj file requirements:
	// - Face elements must specify triangle primitives comprised of three triplets, one triplet for each of the three vertices in a triangle, with each triplet referencing all three vertex attributes, e.g., f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3.
	//
	// Variables used to parse the Wavefront .obj file:
	// Intermediate arrays are used to temporarily store all vertex attributes before they are copied to the external variables OurVertices and OurIndices, which are used to initialize DirectX data structures.
	// Each intermediate array is a one-dimensional array of structures, where each array element (each structure) contains vertex attributes of a given type for one vertex.
	// v[]  is the intermediate array of structures for geometric vertices,			where each structure contains three floating-point values (X, Y, Z).
	// vn[] is the intermediate array of structures for vertex normal vectors,		where each structure contains three floating-point values (x, y, z).
	// vt[] is the intermediate array of structures for vertex texture coordinates, where each structure contains two	floating-point values (x, y; usually named U, V in computer graphics).
	// Each intermediate array is indexed by a variable, vi, vni, or vti, initialized to -1, that is incremented by 1 each time a new vertex attribute is stored in the intermediate array.
	std::vector<XMFLOAT3>* v = nullptr; int vi = -1;		// Geometric vertices			dynamic intermediate array and index (v[vi]).
	std::vector<XMFLOAT3>* vn = nullptr; int vni = -1;		// Vertex normal vectors		dynamic intermediate array and index (vn[vni]).
	std::vector<XMFLOAT2>* vt = nullptr; int vti = -1;		// Vertex texture coordinates	dynamic intermediate array and index (vt[vti]).

//*HERE3*
	// Open the Wavefront .obj file for input.
	obj.open("Text.obj", ios::in);
	// Check whether the Wavefront .obj file opened successfully. (!obj), (!obj.is_open()), and (obj.fail()) all indicate an error opening the file.
	if (!obj)												// If not (!) successful (obj) then:
	{
		// Cannot open the Wavefront .obj file.

		// Terminate this function with a return code indicating an error.
		return 1;
	}

	while (getline(obj, stringtext))						// Attempt to read an entire statement, from the input file stream object obj, into the string variable "stringtext". At eof "getline" becomes false and the while loop is exited.
	{
		// Check whether the statement's first non-blank character is "# ", indicating a comment statement.
//*HERE4* Consider using stringtext.find("#", 0) to find the first occurrence of "#" in stringtext, starting at position 0. This would allow comments to be placed anywhere in the statement, not just at the beginning. This is what the original version of objReader did.
		if (stringtext[0] == '#')							// If the statement's first non - blank character is "# " then:
		{
			// Ignore comments.
			continue;										// Skip the rest of the while loop and continue with the next iteration of the while loop.
		}

		// Check whether the statement's first non-blank character is "v ", indicating a geometric vertex statement, then convert each of three numeric string values to float and store them in v[].
		if (stringtext[0] == 'v')							// If the statement's first non-blank character is "v " then:
		{
			// Process the geometric vertex statement.
			vi = vi + 1;									// Increment the geometric vertex index.
			v[vi].X = stof(stringtext.substr(2, 8));		// Convert the first numeric string value to float and store it in v[vi].X.
			v[vi].Y = stof(stringtext.substr(10, 8));		// Convert the second numeric string value to float and store it in v[vi].Y.
			v[vi].Z = stof(stringtext.substr(18, 8));		// Convert the third numeric string value to float and store it in v[vi].Z.
			continue;										// Skip the rest of the while loop and continue with the next iteration of the while loop.
		}


/*
		If statement's first non-blank character = "vt" Then // Vertex texture coordinates are temporarily stored in vt[], an intermediate array.
		{
			vti = vti + 1
			vt[vti].U = StringtoFloat(Find(1st #))
			vt[vti].V = StringtoFloat(Find(2nd #))
			Continue
		}

		If statement's first non-blank character = "vn" Then // Vertex normal vectors are temporarily stored in vn[], an intermediate array.
		{
			vni = vni + 1
			vn[vni].x = StringtoFloat(Find(1st #))
			vn[vni].y = StringtoFloat(Find(2nd #))
			vn[vni].z = StringtoFloat(Find(3rd #))
			Continue
		}
		
		If statement's first non-blank character = "f " Then // All vertex attributes in the Wavefront .obj file have been processed. Process the next face element and assign values to OurVertices, the vertex attribute array, and OurIndices, the vertex attribute index array.
		{
			For i = 1 to 3 // Process each of the three triplets (v/vt/vn) in the current face element (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3) by extracting face element indices, adjusted by -1. (Adapting the Wavefront .obj file format to DirectX)
			{
				s_no = s_no + 1					  // Update s_no array index (OurVertices[s_no])
				triplet_no = NewValuetriplet_no(triplet_no) // Update triplet_no array index (OurIndices[triplet_no]). NewValuetriplet_no() generates triplet_no in the sequence 0, 2, 1, 3, 5, 4, etc. See the function's definition below. (Adapting the Wavefront .obj file format to DirectX)

				If i = 1 Then
				{
					// i = 1: Process the first triplet (v1/vt1/vn1) in the current face element (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3):
					vi = StringtoFloat(Substring(statement, " ", "/")) -1
					vti = StringtoFloat(Substring(statement, "/", "/")) -1
					vni = StringtoFloat(Substring(statement, "/", " ")) -1
				}

				If i = 2 Then
				{
					// i = 2: Process the second triplet (v2/vt2/vn2) in the current face element (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3):
					vi = StringtoFloat(Substring(statement, " ", "/")) -1
					vti = StringtoFloat(Substring(statement, "/", "/")) -1
					vni = StringtoFloat(Substring(statement, "/", " ")) -1
				}

				If i = 3 Then
				{
					// i = 3: Process the third triplet (v3/vt3/vn3) in the current face element (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3):
					vi = StringtoFloat(Substring(statement, " ", "/")) -1
					vti = StringtoFloat(Substring(statement, "/", "/")) -1
					vni = StringtoFloat(Substring(statement, "/", /EOL)) -1
				}

				// Copy the set of vertex attributes from their intermediate arrays to a new candidate OurVertices array element.
				OurVertices[s_no].X = v[vi].X
				OurVertices[s_no].Y = v[vi].Y
				OurVertices[s_no].Z = v[vi].Z * -1.0f				   // Invert the geometric vertex's Z coordinate.		   (Adapting the Wavefront .obj file format to DirectX)
				OurVertices[s_no].VertexNormal.x = vn[vni].x
				OurVertices[s_no].VertexNormal.y = vn[vni].y
				OurVertices[s_no].VertexNormal.z = vn[vni].z * -1.0f // Invert the vertex normal vector's Z coordinate.	   (Adapting the Wavefront .obj file format to DirectX)
				OurVertices[s_no].U = vt[vti].U
				OurVertices[s_no].V = 1.0f - vt[vti].V			   // Invert the vertex texture coordinate's V coordinate. (Adapting the Wavefront .obj file format to DirectX)

				// Check whether the new OurVertices array element is unique.
				If (OurVertices[s_no] already exists in OurVertices[] at [Index]) Then
				{
					// The new OurVertices[s_no] array element is non-unique (it already exists at OurVertices[Index]) and must be deleted.
				
					// Set OurIndices[triplet_no] to reference the existing OurVertices array element OurVertices[Index].
					OurIndices[triplet_no] = Index // OurVertices[OurIndices[triplet_no]] is OurVertices[Index]
				
					// Delete the non-unique OurVertices array element.
					OurVertices[s_no] = NULL
					s_no = s_no - 1 // This causes the indices s_no and triplet_no to diverge. If all OurVertices array elements are unique then s_no and triplet_no remain synchronized.

				}	Else
					{
						// The new OurVertices[s_no] array element is unique and will be used.

						// Set OurIndices[triplet_no] to reference the new OurVertices array element OurVertices[s_no].
						OurIndices[triplet_no] = s_no // OurVertices[OurIndices[triplet_no]] is OurVertices[s_no]
					}
			}
		}
	}

NewValuetriplet_no(x)
// Converting the drawing order from counter-clockwise (Wavefront .obj file) to clockwise (DirectX): Reversing the order of face element geometric vertex indices. (Adapting the Wavefront .obj file format to DirectX)
{
	If (i = 1) Then
	{
		i = 2 // i is initially 1, therefore the first time this function is called i is set to 2.  i alternates between 2 and 1.
	}	Else
		{
			i = 1
		}
	Return x = x + i, // Increment the parameter by i and return this value to the caller.
	
	The calling program calls the NewValuetriplet_no() function to reverse the order of vertices by generating triplet_no in the sequence 0, 2, 1 etc.
	This is preferable to rewriting OurIndices in reverse order after it is initially computed.
	OurIndices cannot be written in full reverse order from the start, because I do not have a count of the total number of face element triplets until I'm done processing the Wavefront .obj file, 
	and I do not want to "pre-read" the Wavefront .obj file to calculate this count, as this would mean reading the entire file twice.
	Note it is not necessary to "pre-read" the Wavefront .obj file to determine the amount of storage needed to define OurVertices and OurIndices, as they are dynamically allocated using a C++ std::vector.

	NewValuetriplet_no() Function:
	x = 0 (initial value)
	x = NewValuetriplet_no(x) :: x = x + i, where i alternates between 2 and 1.
	By default C++ functions pass by value, in which case the return value needs to be assigned to a variable in the scope of the caller.
	Questions:
	1. If NewValuetriplet_no() is a function then does i have to be static to alternate (if i = 2 then i = 1 else i = 2)?
	2. If NewValuetriplet_no() is not a function (if it is inline) then is i being static immaterial and is this more efficient?

	Reverse only the 2nd and 3rd indices of each triangle's three vertices:
	0, 1, 2,    // Triangle 1
	2, 1, 3     // Triangle 2
	
	Becomes:	
	0, 2, 1,    // Triangle 1 reversed
	2, 3, 1     // Triangle 2 reversed
	
	In this case you're keeping the starting point of each triangle, the first index, and then moving in the opposite direction.
	This method was first seen at https://msdn.microsoft.com/en-us/library/windows/desktop/bb204853(v=vs.85).aspx.
	
}
*/