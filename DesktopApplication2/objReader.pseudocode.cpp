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

// File Stream Functions.
#include <fstream>											// File stream class member functions get, close, etc.

// String stream class member functions.
#include <sstream>											// String stream class member functions getline, etc.

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
using std::istringstream;

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

// Define external variables in one and only one source file (this one) and initialize them as needed.
// See the associated header file for declarations and descriptions of these external variables.
vector<VERTEX>* OurVertices; int OurVerticesi = -1;
vector<DWORD>* OurIndices; int OurIndicesi = -1;

// End: External Variable Global Definitions.

// End: Global Declarations.

//***
// Functions and local variables follow.
//***

// objReader function: Definition
int objReader(void)
{
	//***
	// Local Declarations.
	//***

	ifstream obj;											// Declare the input file stream object representing the Wavefront .obj file.
	string stringtext;										// Holds one statement of the input file stream object representing the Wavefront .obj file.

	// Wavefront .obj file format requirements:
	// - Vertex attribute statements are listed first, and will therefore be parsed before any face element statements.
	//   Supported and required vertex attribute statements are geometric vertex statements (v x y z), vertex texture coordinate statements (vt u v), and vertex normal vector statements (vn x y z).
	//   Face element statements specify indices referring to vertex attribute statements by the order in which the vertex attribute statements appear.
	//   Supported and required face element statements specify three triplets, one triplet for each of the three vertices of a triangle (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3), where v1, v2, v3 are geometric vertex indices, vt1, vt2, vt3 are vertex texture coordinate indices, and vn1, vn2, vn3 are vertex normal vector indices.
	//   The order of the face element statements determines the order in which the triangles must be drawn. This order is important when dealing with overlapping triangles, as the later triangles will be drawn on top of the earlier ones.
	// - No spaces are permitted before or after a slash ('/').
	// - Statements can start in any column.
	// - Statements can be logically joined with the line continuation character ( \ ) at the end of a line. (*This is not supported by this program*)
	//
	// Variables used to parse the Wavefront .obj file: Vertex attribute intermediate arrays
	// Intermediate arrays are used to temporarily store all vertex attributes before they are copied to the array variables OurVertices and OurIndices, which are used to initialize DirectX data structures.
	// Each intermediate array is a one-dimensional array of structures, where each array element (each structure) contains vertex attributes of a given type for one vertex.
	// v  is the intermediate array of structures for geometric vertices,		  where each structure contains three floating-point values (x, y, z).
	// vt is the intermediate array of structures for vertex texture coordinates, where each structure contains two   floating-point values (x, y; usually named U, V in computer graphics).
	// vn is the intermediate array of structures for vertex normal vectors,	  where each structure contains three floating-point values (x, y, z).
	// Each intermediate array is indexed by a variable, vi, vni, or vti, initialized to -1, that is incremented by 1 each time a new vertex attribute is stored in the intermediate array.
	vector<XMFLOAT3> v; int vi = -1;					// Geometric vertices		  dynamically allocated intermediate array, and index (v[vi]).
	vector<XMFLOAT2> vt; int vti = -1;					// Vertex texture coordinates dynamically allocated intermediate array, and index (vt[vti]).
	vector<XMFLOAT3> vn; int vni = -1;					// Vertex normal vectors	  dynamically allocated intermediate array, and index (vn[vni]).

	// Open the Wavefront .obj file for input.
	obj.open("Text.obj", ios::in);
	// Check whether the Wavefront .obj file opened successfully. (!obj), (!obj.is_open()), and (obj.fail()) all indicate an error opening the file.
	if (!obj)												// If not (!) successful (obj) then:
	{
		// Cannot open the Wavefront .obj file.

		// Terminate this function with a return code indicating an error.
		return 1;
	}

	while (getline(obj, stringtext))						// Read an entire statement, from the input file stream object obj, into the string variable stringtext. At eof getline becomes false and the while loop is exited.
	{
		istringstream lineStream(stringtext);				// Declaring object "lineStream" inside the loop ensures that it is reset and ready to parse the next line of the file in each iteration of the loop.
		string type;										// Wavefront .obj file statement type: #, v, vn, vt, f, etc.
															// Declaring variable "type" inside the loop ensures that it is reset at the start of each iteration, which is generally what you want when reading a new line from the file.
		// Face element vertex attribute indices (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3) of the current face element triplet.
		int fv;												// Geometric vertex index (v1, or v2, or v3).
		int fvt;											// Vertex texture coordinate index (vt1, or vt2, or vt3).
		int fvn;											// Vertex normal vector index (vn1, or vn2, or vn3).
		char slash = '/';									// The slash character ('/') is a delimiter used to parse the face element.

		lineStream >> type;									// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "type".

		if (type == "v")
		{
			// The statement read is a geometric vertex statement. Parse and store it in variable v.
			// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "v[vi].x", then "v[vi].y", then "v[vi].z".
			vi++;												// Increment the geometric vertex index.
			lineStream >> v[vi].x >> v[vi].y >> v[vi].z;		// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "v[vi].x", then "v[vi].y", then "v[vi].z".
		} else if (type == "vn")
		{
			// The statement read is a vertex normal vector statement. Parse and store it in variable vn.
			// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "vn[vni].x", then "vn[vni].y", then "vn[vni].z".
			vni++;												// Increment the vertex normal vector index.
			lineStream >> vn[vni].x >> vn[vni].y >> vn[vni].z;	// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "vn[vni].x", then "vn[vni].y", then "vn[vni].z".
		} else if (type == "vt")
		{
			// The statement read is a vertex texture coordinate statement. Parse and store it in variable vt.
			// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "vt[vti].x", then "vt[vti].y".
			vti++;												// Increment the vertex texture coordinate index.
			lineStream >> vt[vti].x >> vt[vti].y;				// The >> operator extracts the next value from the lineStream input stream object and stores it in the variable "vt[vti].x", then "vt[vti].y".
		} else if (type == "f")
		{
			// The statement read is a face element statement, therefore all vertex attribute statements in the Wavefront .obj file have been read and parsed.
			// Now parse the current face element statement:
			//   Each triplet consists of three pointers to three vertex attributes in the Wavefront .obj file, which together comprise one set of vertex attributes for one of the three vertices of a triangle. The set of vertex attributes is stored in the next sequential element of the array variable OurVertices, which is pointed to by array variable OurIndices.
			//   This results in storing vertex attributes in array variable OurVertices in the order specified by the face element statements, which is the order in which the triangles must be drawn.
			//   However, the drawing order of triangle vertices must be converted from counter-clockwise (Wavefront .obj file) to clockwise (DirectX), to adjust it from the Wavefront .obj file format to the DirectX format.
			//   This can be done by reversing the order of processing the second and third set of vertex attributes of each triangle's three vertices, which is equivalent to changing the order by which the elements of array variable OurVertices are referenced by array variable OurIndices:
			//     OurVertices[OurVerticesi] is OurVertices[OurIndices[OurIndicesi]], where instead of OurIndices[OurIndicesi] = 0, 1, 2, 3, 4, 5, etc. the drawing order is reversed using OurIndices[OurIndicesi] = (-2,) 0, 2, 1, 3, 5, 4, 6, 8, 7, 9, 11, 10, etc. Note the nonexistent OurIndices[-1] = -2 is a conceptual "seed" value for the algorithm described below.
			//     Here is the algorithm producing this pattern: It's used starting at Step #2 to calculate OurIndices[1] = 2. OurIndices[0] = 0 is hardcoded instead of calculated.
			//       Step #1: Add		 2 to the first   value to calculate the second value, then
			//       Step #2: Add		 2 to the second  value to calculate the third	value, then
			//       Step #3: Subtract 1 from the third value to calculate the forth	value, then
			//       Repeat, treating the forth value as the new "first value".
			//   Alternatively, the elements of the array variable OurVertices could be reordered while keeping the elements of the array variable OurIndices = 0, 1, 2, 3, 4, 5, etc. (This is not done in this program)

			// Parse each of the three triplets in the face element (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3).
			for (int i = 0; i <= 2; i++)
			{
				// Parse the current triplet (v/vt/vn) in the face element, storing the face element indices.
				lineStream >> fv >> slash >> fvt >> slash >> fvn;

				// Decrement the face element indices by 1 to adjust them from the Wavefront .obj file format to the C++ format.
				// Indices in the Wavefront .obj file format start with 1, while indices in the DirectX format start with 0. However, this program does not use the face element indices directly, so an adjustment *for DirectX* is not necessary.
				// Nevertheless, the face element indices are decremented by 1 because the index variables v1, vti, and vni of the vertex attribute intermediate array variables v[vi], vt[vti], and vn[vn1] start with 0.
				fv--; fvt--; fvn--;

				// Update the indices of array variables OurVertices[OurVerticesi] and OurIndices[OurIndicesi].
				OurVerticesi++;
				OurIndicesi++;
				
				// Use the algorithm described above to calculate the non-sequential value of the next sequential element of the array variable OurIndices, e.g., OurIndices[0] = 0 , OurIndices[1] = 2, OurIndices[2] = 1, OurIndices[3] = 3, OurIndices[4] = 5, OurIndices[5] = 4, etc.
				int delta[3] = { 2, 2, -1 };				// The algorithm described above is implemented using the array variable delta.
				if (OurIndicesi != 0)
				{
					OurIndices->at(OurIndicesi) = OurIndices->at(OurIndicesi - 1) + delta[i]; // OurIndices[OurIndicesi] = OurIndices[OurIndicesi - 1] + delta[i]; starting with OurIndices[1] = OurIndices[1 - 1] + delta[1] == OurIndices[0] + 2 == 0 + 2 == 2 (that is, OurIndices[1] = 2).
				}
				else										// (OurIndicesi != 0)
				{
					OurIndices->at(OurIndicesi) = 0;		// OurIndices[0] = 0 is hardcoded instead of calculated.
				}
				// Alternatively, use the algorithm described above to calculate the non-sequential index of the next sequential value of the array variable OurIndices, e.g., OurIndices[0] = 0 , OurIndices[2] = 1, OurIndices[1] = 2, OurIndices[3] = 3, OurIndices[5] = 4, OurIndices[4] = 5, etc. (This is not done in this program)

				// Test if the candidate set of vertex attributes is unique, i.e., has this set of vertex attributes been previously found in the Wavefront .obj file and stored in array variable OurVertices?
				// This is done to avoid storing duplicate sets of vertex attributes in array variable OurVertices.
				// Define a semaphore to indicate whether the candidate set of vertex attributes is unique.
				bool unique = true;
				for (int j = 0; j < OurVerticesi; j++)
				{
					if (OurVertices->at(j).GeometricVertex.x == v[fv].x &&
						OurVertices->at(j).GeometricVertex.y == v[fv].y &&
						OurVertices->at(j).GeometricVertex.z == v[fv].z &&
						OurVertices->at(j).VertexTextureCoordinate.x == vt[fvt].x &&
						OurVertices->at(j).VertexTextureCoordinate.y == 1.0f - vt[fvt].y &&
						OurVertices->at(j).VertexNormalVector.x == vn[fvn].x &&
						OurVertices->at(j).VertexNormalVector.y == vn[fvn].y &&
						OurVertices->at(j).VertexNormalVector.z == vn[fvn].z * -1.0f)
					{
						// The candidate set of vertex attributes is not unique.
						unique = false;
						OurIndices->at(OurIndicesi) = j;	// Assign to array variable OurIndices[OurIndicesi] the index (the value of variable j) of the element of array variable OurVertices[j], which contains the set of vertex attributes previously found.
						OurVerticesi--;
						break;
					}
				}
				// If the candidate set of vertex attributes is unique, it is stored in the array variable OurVertices.
				if (unique)
				{
					// Assign the candidate set of vertex attributes, specified by the indices of the current face element triplet, from the intermediate array variables v, vt, and vn to the array variable OurVertices.
					//
					// The order of the face element statements in the Wavefront .obj file determines the order in which vertices must be drawn. Face element statements are parsed in this order.
					// Each set of vertex attributes is stored in the next sequential element of array variable OurVertices, unless it is not unique as discussed above.
					// When rendering an object with DirectX, the drawing order of vertices is determined by the values of the sequential elements of the array variable OurIndices.
					// The values of these elements are the indices of array variable OurVertices, and would also be sequential, were it not for 1) the need to convert the drawing order to clockwise as discussed above, and 2) the need to avoid storing duplicate sets of vertex attributes in array variable OurVertices as discussed above.
					OurVertices->at(OurVerticesi).GeometricVertex.x = v[fv].x;
					OurVertices->at(OurVerticesi).GeometricVertex.y = v[fv].y;
					OurVertices->at(OurVerticesi).GeometricVertex.z = v[fv].z * -1.0f;			// Invert the geometric vertex's Z coordinate, to adjust it from the Wavefront .obj file format to the DirectX format.

					OurVertices->at(OurVerticesi).VertexTextureCoordinate.x = vt[fvt].x;
					OurVertices->at(OurVerticesi).VertexTextureCoordinate.y = 1.0f - vt[fvt].y;	// Invert the vertex texture coordinate's V coordinate, to adjust it from the Wavefront .obj file format to the DirectX format.

					OurVertices->at(OurVerticesi).VertexNormalVector.x = vn[fvn].x;
					OurVertices->at(OurVerticesi).VertexNormalVector.y = vn[fvn].y;
					OurVertices->at(OurVerticesi).VertexNormalVector.z = vn[fvn].z * -1.0f;		// Invert the vertex normal vector's Z coordinate, to adjust it the Wavefront .obj file format to the DirectX format.
				}
			}
		} else continue;									// The statement read is not a geometric vertex, vertex texture coordinate, vertex normal vector, or face element statement. Ignore it and continue.
	}														// End of the while loop. The entire Wavefront .obj file has been read and parsed. Return to the calling program.
}