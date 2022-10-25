// objReader
// Version 3.1
//
// Description
// This function parses a Wavefront .obj file for a single 3D object's descriptive information and assigns it to the external global variables used by the calling program when rendering the object.
//
// Authorship
// Robert John Tortorelli

//***
// Global Declarations.
//***

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
// External Global Variables.
//***

// Wavefront .obj file I/O (objReader function) Header File.
#include "objReader.h"										// Declare external global variables in this header file, and include it in all source files that reference these external global variables.
// Define external global variables in one and only one source file, this one, and initialize them as needed.
// See the associated header file for descriptions of these global variables.
VERTEX* OurVertices;
int GeometricVerticesTotal = 0;
DWORD* OurIndices;
int PrimitivesTotal = 0;

// End: External Global Variables.

// End: Global Declarations.

// objReader function: Definition
//   This function parses a Wavefront .obj file for a single 3D object's descriptive information and assigns it to the variables used by the calling program when rendering the object.
//   A subset of all Wavefront descriptive elements are parsed.
//     1. Parse the Wavefront .obj file: Count the number of occurrences of each descriptive element in the Wavefront .obj file.
//
//     2. Use the count of the number of occurrences of each descriptive element to calculate and allocate memory for the external global variables that will store them.
//
//     3. Parse the Wavefront .obj file: Assign each occurrence of each descriptive element in the Wavefront .obj file to its associated external global variable.
int objReader(void)
{
	//***
	// Local Declarations.
	//***

	ifstream obj;											// Declare the input file stream object representing the Wavefront .obj file.
	string stringtext;										// Holds one line of the input file stream object representing the Wavefront .obj file.
	// size_t is a type able to represent the size of any object in bytes.
	// It is the type returned by the "sizeof" operator and is widely used in the standard library to represent sizes and counts.
	size_t nCharCounter = 0;								// Index (>= 0) into a vertex element or face element in a line of the Wavefront .obj file.
	size_t nOffset;											// Index (>= 0) into a line of the Wavefront .obj file.
	// string::iterator is a member type that acts like a pointer to a character in a string, or an index of an array, but without assuming the container you are iterating over has the random-access capability of an array, i.e., you have container independence.
	// It is the member type returned by many string member functions.
	string::iterator it;									// Location within a line of the Wavefront .obj file.

	// Three vertex indices, each a face element pointing to a vertex comprised of three vertex elements, describe a face; i.e., a triangle primitive.
	const string v("v ");									// "v " prefix indicating a line of the Wavefront .obj file containing three geometric vertex elements describing one vertex of an object.
	const string f("f ");									// "f " prefix indicating a line of the Wavefront .obj file containing three polygonal face elements   describing one face   of an object.

	#define MAX_ELEMENT_SIZE 50								// Define the maximum vertex element and face element size.
	char tempchar[MAX_ELEMENT_SIZE] = { '\0' };				// Holds one complete vertex element or face element, null-terminated by initializing all array elements to the null-terminator.
	int ignoreFlag = 0;										// Only save characters of a face element when ignoreFlag = 0.

	// Variables used to track which vertex and vertex element is being parsed in the Wavefront .obj file.
	int VertexNo = -1;										// The number of the vertex being parsed, counting from 0. This corresponds to the vertex index (>= 0), a face element.
	int VertexElementNo;									// The number of the vertex element of the vertex being parsed: x (VertexElementNo = 1), y (VertexElementNo = 2) or z (VertexElementNo = 3).

	// Variables used to track which face and face element is being parsed in the Wavefront .obj file.
	int FaceElementNo = 0;									// The number of the vertex index, a face element, being parsed, counting from 0. It's counted relative to the first vertex index of the first face, sequentially across all faces. Its final value is one greater than the final value used.

	// End: Local Declarations.

	//***
	// 1. Parse the Wavefront .obj file: Count the number of occurrences of each descriptive element in the Wavefront .obj file.
	//***

	// Open the Wavefront .obj file for input.
	obj.open("Text.obj", ios::in);
	// Check whether the Wavefront .obj file opened successfully. (!obj), (!obj.is_open()), and (obj.fail()) all indicate an error opening the file.
	if (!obj)												// If not (!) successful (obj) then:
	{
		// Cannot open the Wavefront .obj file.

		// Terminate this function with a return code indicating an error.
		return 1;
	}

	// Count the total number of geometric vertices (GeometricVerticesTotal), and the total number of primitives (PrimitivesTotal), of the object specified in the Wavefront .obj file.
	while (getline(obj, stringtext))						// Attempt to read an entire line, from the input file stream object obj, into the string variable "stringtext". At eof getline becomes false and the while loop is exited.
	{
		// A line was successfully read from the Wavefront .obj file.

		// stringtext.find(v, 0) returns the position (0 - npos) of the first character of the first match of the string v in "stringtext", starting from the default position 0. If no match is found it returns npos.
		// npos is a constant static member value with the greatest possible value for an element of type size_t.
		if ((nOffset = stringtext.find(v, 0)) != string::npos)
		{
			// A "v " geometric vertex line has been found in the Wavefront .obj file.

			++GeometricVerticesTotal;						// Count the geometric vertex line that was found.
		}
		else if ((nOffset = stringtext.find(f, 0)) != string::npos)
		{
			// A "f " face element line		has been found in the Wavefront .obj file.

			++PrimitivesTotal;								// Count the face element line	   that was found.
		}
	}
	// A line was not successfully read from the Wavefront .obj file. The end of the file (eof) has been reached.

	// End: 1. Parse the Wavefront.obj file : Count the number of occurrences of each descriptive element in the Wavefront.obj file.

	//***
	// 2. Use the count of the number of occurrences of each descriptive element to calculate and allocate memory for the external global variables that will store them.
	//***

	// Allocate memory for OurVertices and OurIndices.
	OurVertices = new VERTEX[GeometricVerticesTotal];
	OurIndices = new DWORD[PrimitivesTotal * 3];

	// End: 2. Use the count of the number of occurrences of each descriptive element to calculate and allocate memory for the external global variables that will store them.

	//***
	// 3. Parse the Wavefront .obj file: Assign each occurrence of each descriptive element in the Wavefront .obj file to its associated external global variable.
	//***

	// Prepare to reread from the Wavefront .obj file.
	obj.clear();											// The Wavefront .obj file has already been read to eof, so the input file stream's internal error state flags indicate eof. Therefore all subsequent input operations fail unless the input file stream's flags are first cleared to goodbit.
	obj.seekg(0, ios::beg);									// Set the position of the next character to be read to the input file stream's beginning.

	// Assign values to OurVertices and OurIndices from the Wavefront .obj file.
	while (getline(obj, stringtext))						// Attempt to read an entire line, from the input file stream object obj, into the string variable "stringtext". At eof getline becomes false and the while loop is exited.
	{
		// A line was successfully read from file obj.

		// ++nOffset in the following for loops:
		// Keywords "v " and "f " are expected in column 1 (offset 0, as returned by the find function) of the line. This program expects any associated values (such as vertex data) starting in column 3 (offset 2). Therefore, nOffset is incremented, before entering each of the following two for loops, to point to column 2 (offset 1), where a " " is expected. As a result, after nOffset is first incremented inside the following for loops it is pointing to any associated values in column 3 (offset 2).

		if ((nOffset = stringtext.find(v, 0)) != string::npos)
		{
			// A "v " geometric vertex line has been found in the Wavefront .obj file.

			++VertexNo;										// We are parsing a new Vertex.
			VertexElementNo = 1;							// We are parsing the first vertex element, X, of a new Vertex.

			// Initialize the Vertex's Color using an arbitrary formula:
			// Color[x] is 0 or 1 because (n MOD 2) is 0 or 1 for any integer n.
			// Suppress: "Warning C6386 Buffer overrun while writing to 'OurVertices'. Invalid write to 'OurVertices[1]', (writable range is 0 to 0)"
			#pragma warning(suppress : 6386)
			// This is a false positive. Reason:
			// Array element numbers range from 0 to the size of the array - 1.
			// The compiler cannot verify that the array element number VertexNo is never more than the size of the array - 1, i.e, VertexNo <= GeometricVerticesTotal - 1.
			OurVertices[VertexNo].Color[0] = (float)((VertexNo / 1) % 2);
			OurVertices[VertexNo].Color[1] = (float)((VertexNo / 2) % 2);
			OurVertices[VertexNo].Color[2] = (float)((VertexNo / 3) % 2);
			OurVertices[VertexNo].Color[3] = (float)((VertexNo / 4) % 2);

			// Parse the current "v " vertex line in the Wavefront .obj file for vertex elements, and store them.

			++nOffset;
			for (it = stringtext.begin() + ++nOffset; it != stringtext.end(); ++it)
			{
				if (*it != ' ')
				{
					// Not ' ' indicates a digit of a vertex element. Initially *it is pointing to the first non-blank value after "v ".

					tempchar[nCharCounter] = *it;
					++nCharCounter;
				}
				else
				{
					// ' ' char detected, which indicates a complete vertex element has been read into "tempchar".

					switch (VertexElementNo)
					{
						//*** case 1 may execute ***
					case 1:	OurVertices[VertexNo].X = stof(tempchar);
						break;
						//*** case 2 may execute ***
					case 2:	OurVertices[VertexNo].Y = stof(tempchar);
						break;
						//*** case 3 will never execute (because end of line, not ' ', indicates the 3rd element) ***
					case 3:	OurVertices[VertexNo].Z = stof(tempchar);
						// Invert each vertices' Z coordinate.
						OurVertices[VertexNo].Z = OurVertices[VertexNo].Z * -1.0f;
						break;
					}

					// Clear "tempchar" with the null-terminator.
					fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
					// Reset "tempchar's" array element pointer.
					nCharCounter = 0;
					// Prepare to parse the next vertex element, y or z.
					++VertexElementNo;
				}
			}
			// End of line reached, which indicates this line's final complete vertex element has been read into "tempchar".

			switch (VertexElementNo)
			{
				//*** case 1 will never execute (because ' ', not end of line, indicates the 1st or 2nd element) ***
			case 1:	OurVertices[VertexNo].X = stof(tempchar);
				break;
				//*** case 2 will never execute (because ' ', not end of line, indicates the 1st or 2nd element) ***
			case 2:	OurVertices[VertexNo].Y = stof(tempchar);
				break;
				//*** case 3 may execute ***
			case 3:	OurVertices[VertexNo].Z = stof(tempchar);
				// Invert each vertices' Z coordinate.
				OurVertices[VertexNo].Z = OurVertices[VertexNo].Z * -1.0f;
				break;
			}

			// Clear "tempchar" with the null-terminator.
			fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
			// Reset "tempchar's" array element pointer.
			nCharCounter = 0;
			// Get another line from the while loop.
		}
		else if ((nOffset = stringtext.find(f, 0)) != string::npos)
		{
			// A "f " face element line		has been found in the Wavefront .obj file.

			// Parse the current "f " face line in the Wavefront .obj file for face elements, and store them.
			++nOffset;
			for (it = stringtext.begin() + ++nOffset; it != stringtext.end(); ++it)
			{
				if (*it != ' ' && *it != '/')
				{
					// Not ' ' AND Not '/' indicates a digit of a face element.

					if (ignoreFlag == 0)
					{
						// Save characters of the face element, as it's a vertex index.

						tempchar[nCharCounter] = *it;
						++nCharCounter;
					}
				}
				else
				{
					// ' ' OR '/' char detected:
					// ' ' demarks a vertex index, or	a vertex Texture Coordinate Index, or		a vertex Normal Index.
					// '/' demarks a vertex index, or	a vertex Texture Coordinate Index, but not	a vertex Normal Index.
					// '/' indicates					a vertex Texture Coordinate Index, or		a vertex Normal Index follows.

					if (*it == ' ')
					{
						// ' ' char detected, which indicates a complete face element, a vertex index, has been read into "tempchar" (ignoreFlag ensures "tempchar" only holds a vertex index).

						// Convert (reverse) the triangle primitive drawing order from counter-clockwise (used by the Wavefront .obj specification) to clockwise (used by DirectX).
						// "OurIndices[FaceElementNo] = strtol(tempchar, NULL, 10);"			// Store the face element, a vertex index.																					   *** Order of vertices is unchanged ***
						OurIndices[(PrimitivesTotal * 3) - FaceElementNo - 1] = strtol(tempchar, NULL, 10);	// Store the face element, a vertex index.																		   *** Order of vertices is reversed  ***
						// --OurIndices[FaceElementNo];											// DirectX index buffer values start with 0, while the equivalent Wavefront .obj face index values start with 1. Decrement it. *** Order of vertices is unchanged ***
						--OurIndices[(PrimitivesTotal * 3) - FaceElementNo - 1];				// DirectX index buffer values start with 0, while the equivalent Wavefront .obj face index values start with 1. Decrement it. *** Order of vertices is reversed  ***
						fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');						// Clear "tempchar" with the null-terminator.
						nCharCounter = 0;														// Reset "tempchar's" array element pointer.
						ignoreFlag = 0;															// Reset the flag indicator to save characters of the face element, as it's a vertex index.
						++FaceElementNo;														// Prepare to parse the next face element.
					}
					else
					{
						// '/' char detected, which indicates a vertex Texture Coordinate Index or a vertex Normal Index follows. We are only interested in a vertex index.

						ignoreFlag = 1;															// Set the flag indicator to not save characters of the face element, because it's not a vertex index.
					}
				}
			}
			// End of line reached, which indicates this line's final complete face element has been read into "tempchar".

			// Convert (reverse) the triangle primitive drawing order from counter-clockwise (used by the Wavefront .obj specification) to clockwise (used by DirectX).
			// "OurIndices[FaceElementNo] = strtol(tempchar, NULL, 10);"						// Store the face element, a vertex index.																					   *** Order of vertices is unchanged ***
			OurIndices[(PrimitivesTotal * 3) - FaceElementNo - 1] = strtol(tempchar, NULL, 10);	// Store the face element, a vertex index.																					   *** Order of vertices is reversed  ***
			// --OurIndices[FaceElementNo];														// DirectX index buffer values start with 0, while the equivalent Wavefront .obj face index values start with 1. Decrement it. *** Order of vertices is unchanged ***
			--OurIndices[(PrimitivesTotal * 3) - FaceElementNo - 1];							// DirectX index buffer values start with 0, while the equivalent Wavefront .obj face index values start with 1. Decrement it. *** Order of vertices is reversed  ***
			fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');									// Clear "tempchar" with the null-terminator.
			nCharCounter = 0;																	// Reset "tempchar's" array element pointer.
			ignoreFlag = 0;																		// Reset the flag indicator to save characters of a face element.
			++FaceElementNo;																	// Prepare to parse the next face element. Because it's incremented when the end of the line is reached, its final value is one greater than the final value used.
			// Get another line from the while loop.
		}
	}

	// Close the Wavefront .obj file.
	obj.close();

	// End: 3. Parse the Wavefront.obj file : Assign each occurrence of each descriptive element in the Wavefront.obj file to its associated external global variable.

	// Terminate this function with a return code indicating success.
	return 0;
}