// Temporary copy from project ConsoleApplication1, which will be the basis for the version of objReader that supports geometric vertices, vertex texture coordinates, and vertex normal vectors.
//
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
	string stringtext;										// Holds one statement of the input file stream object representing the Wavefront .obj file.
	// size_t is a type able to represent the size of any object in bytes.
	// It is the type returned by the "sizeof" operator and is widely used in the standard library to represent sizes and counts.
	size_t nCharCounter = 0;								// Index (>= 0) into a subcomponent value of an element of the Wavefront .obj file.
	size_t nOffset;											// Index (>= 0) into a statement of the Wavefront .obj file.
	// string::iterator is a member type that acts like a pointer to a character in a string, or an index of an array, but without assuming the container you are iterating over has the random-access capability of an array, i.e., you have container independence.
	// It is the member type returned by many string member functions.
	string::iterator it;									// Location within a statement of the Wavefront .obj file.

	const string f("f ");									// "f " prefix indicating a statement of the Wavefront .obj file defining one face element.
	const string v("v ");									// "v " prefix indicating a statement of the Wavefront .obj file defining one geometric vertex element.

	#define MAX_ELEMENT_SIZE 50								// Define the maximum size of one complete subcomponent value of an element, e.g., the X coordinate of a geometric vertex element.
	char tempchar[MAX_ELEMENT_SIZE] = { '\0' };				// Holds one incomplete or complete subcomponent value of an element, null-terminated by initializing all array elements to the null-terminator.
	int ignoreFlag = 0;										// Only save characters of a face element when ignoreFlag = 0.

	// Variables used to track which geometric vertex element and geometric vertex coordinate is being parsed in the Wavefront .obj file.
	int VertexNo = -1;										// The number of the geometric vertex element being parsed, counting from 0. This corresponds to the geometric vertex index (>= 0), a face element.
	int VertexCoordinateNo;									// The number of the geometric vertex coordinate being parsed: x (VertexCoordinateNo = 1), y (VertexCoordinateNo = 2) or z (VertexCoordinateNo = 3). It is reset for each geometric vertex element parsed.

	int FaceIndexNo = 0;									// The number geometric vertex indices processed, counting from 0. It's counted relative to the first geometric vertex index of the first face element, sequentially across all face elements. Its final value is one greater than the final value used.

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
	while (getstatement(obj, stringtext))						// Attempt to read an entire statement, from the input file stream object obj, into the string variable "stringtext". At eof "getstatement" becomes false and the while loop is exited.
	{
		// A statement was successfully read from the Wavefront .obj file.

		// stringtext.find(v, 0) returns the position (0 - npos) of the first character of the first match of the string v in "stringtext", starting from the default position 0. If no match is found it returns npos.
		// npos is a constant static member value with the greatest possible value for an element of type size_t.
		if ((nOffset = stringtext.find(v, 0)) != string::npos)
		{
			// A "v " geometric vertex element has been found in the Wavefront .obj file.

			++GeometricVerticesTotal;						// Count the geometric vertex element that was found.
		}
		else if ((nOffset = stringtext.find(f, 0)) != string::npos)
		{
			// A "f " face element			   has been found in the Wavefront .obj file.

			++PrimitivesTotal;								// Count the face element			  that was found.
		}
	}
	// A statement was not successfully read from the Wavefront .obj file. The end of the file (eof) has been reached.

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
	while (getstatement(obj, stringtext))						// Attempt to read an entire statement, from the input file stream object obj, into the string variable "stringtext". At eof "getstatement" becomes false and the while loop is exited.
	{
		// A statement was successfully read from file obj.

		// ++nOffset in the following for loops:
		// Keywords "v " and "f " are expected in column 1 (offset 0, as returned by the find function) of the statement. This program expects any associated values starting in column 3 (offset 2). Therefore, nOffset is incremented, before entering each of the following two for loops, to point to column 2 (offset 1), where a " " is expected. As a result, after nOffset is first incremented inside the following for loops it is pointing to any associated values in column 3 (offset 2).

		if ((nOffset = stringtext.find(v, 0)) != string::npos)
		{
			// A "v " geometric vertex element has been found in the Wavefront .obj file.

			// Parse the current "v " geometric vertex element in the Wavefront .obj file and store it.

			++VertexNo;										// We are parsing a single new geometric vertex element in the statement read.
			VertexCoordinateNo = 1;							// We are parsing the first of three geometric vertex coordinates, X.

			++nOffset;										// nOffset is incremented, before entering the following for loop, to point to column 2 (offset 1), where a " " is expected. As a result, after nOffset is first incremented inside the following for loop it is pointing to any associated values in column 3 (offset 2).
			for (it = stringtext.begin() + ++nOffset; it != stringtext.end(); ++it)
			{
				// End of statement not yet reached, which indicates a geometric vertex element is being processed.

				if (*it != ' ')
				{
					// Not ' ' indicates a digit of a geometric vertex element. Initially *it is pointing to the first non-blank value after "v ".
					// Save characters of the geometric vertex element, as it's a subcomponent value of a geometric vertex coordinate, in "tempchar". Then update "tempchar's" array element pointer.

					tempchar[nCharCounter] = *it;
					++nCharCounter;
				}
				else
				{
					// ' ' char detected, which indicates a complete subcomponent value of a geometric vertex element has been read into "tempchar".		  Assign "tempchar" to the correct array element of OurVertices[].

					// ' ' indicates the 1st or 2nd geometric vertex coordinate has been read into "tempchar".
					// Store the geometric vertex coordinate.
					switch (VertexCoordinateNo)
					{
					case 1:	OurVertices[VertexNo].X = stof(tempchar);
						break;
					case 2:	OurVertices[VertexNo].Y = stof(tempchar);
						break;
					}

					// Prepare to parse the next geometric vertex coordinate, Y or Z.
					// Clear "tempchar" with the null-terminator.
					fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
					// Reset "tempchar's" array element pointer.
					nCharCounter = 0;
					// Increment the number of the geometric vertex coordinate being parsed.
					++VertexCoordinateNo;
				}
			}
			// End of statement reached, which indicates the final, complete, subcomponent value of a geometric vertex element has been read into "tempchar". Assign "tempchar" to the correct array element of OurVertices[].

			// End of statement indicates the 3rd geometric vertex coordinate, Z, has been read into "tempchar".
			// Store the complete subcomponent value of the geometric vertex coordinate.
			OurVertices[VertexNo].Z = stof(tempchar);
			// Convert (invert) the Wavefront .obj geometric vertices' Z coordinate for DirectX. *** Invert the Wavefront .obj geometric vertices' Z coordinate ***
			OurVertices[VertexNo].Z = OurVertices[VertexNo].Z * -1.0f;

			// Prepare to parse the next statement.
			// Clear "tempchar" with the null-terminator.
			fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
			// Reset "tempchar's" array element pointer.
			nCharCounter = 0;

			// The current "v " geometric vertex element has been parsed. Get another statement from the while loop.
		}
		else if ((nOffset = stringtext.find(f, 0)) != string::npos)
		{
			// A "f " face element has been found in the Wavefront .obj file.

			// Parse the current "f " face element in the Wavefront .obj file and store it.

			++nOffset;										// nOffset is incremented, before entering the following for loop, to point to column 2 (offset 1), where a " " is expected. As a result, after nOffset is first incremented inside the following for loop it is pointing to any associated values in column 3 (offset 2).
			for (it = stringtext.begin() + ++nOffset; it != stringtext.end(); ++it)
			{
				// End of statement not yet reached, which indicates a face element is being processed.

				if (*it != ' ' && *it != '/')
				{
					// Not ' ' AND Not '/' indicates a digit of a face element index.

					if (ignoreFlag == 0)
					{
						// Save characters of the face element, as it's a subcomponent value of a geometric vertex index, in "tempchar". Then update "tempchar's" array element pointer.

						tempchar[nCharCounter] = *it;
						++nCharCounter;
					}
				}
				else
				{
					// ' ' OR '/' char detected:
					// ' ' demarks a geometric vertex index, or	a vertex texture coordinate index, or		a vertex normal vector index.
					// '/' demarks a geometric vertex index, or	a vertex texture coordinate index, but not	a vertex normal vector index.
					// '/' indicates either						a vertex texture coordinate index, or		a vertex normal vector index follows.

					if (*it == ' ')
					{
						// ' ' char detected, which indicates a complete subcomponent value of a face element, a geometric vertex index, has been read into "tempchar" (ignoreFlag ensures "tempchar" only holds a geometric vertex index). Assign "tempchar" to the correct array element of OurIndices[].

						// Store the face element, a geometric vertex index.
						// Convert (reverse) the triangle primitive drawing order from counter-clockwise (used by the Wavefront .obj specification) to clockwise (used by DirectX). *** Reverse the Wavefront .obj order of geometric vertices ***
						OurIndices[(PrimitivesTotal * 3) - FaceIndexNo - 1] = strtol(tempchar, NULL, 10);
						// Wavefront .obj face index values start with 1, while the equivalent DirectX index buffer values start with 0.											*** Decrement the Wavefront .obj face index values ***
						--OurIndices[(PrimitivesTotal * 3) - FaceIndexNo - 1];

						// Prepare to parse the next face element.
						// Clear "tempchar" with the null-terminator.
						fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
						// Reset "tempchar's" array element pointer.
						nCharCounter = 0;
						// Reset the flag indicator to save characters of a face element.
						ignoreFlag = 0;
						// Increment the number geometric vertex indices processed. Because it's incremented when the end of the statement is reached, its final value is one greater than the final value used.
						++FaceIndexNo;
					}
					else
					{
						// '/' char detected, which indicates a vertex texture coordinate index or a vertex normal vector index follows. We are only interested in a geometric vertex index.

						// Set the flag indicator to not save characters of the face element, because it's not a geometric vertex index.
						ignoreFlag = 1;
					}
				}
			}
			// End of statement reached, which indicates the final, complete, subcomponent value of a face element has been read into "tempchar".																							Assign "tempchar" to the correct array element of OurIndices[].

			// Store the complete subcomponent value of the face element, a geometric vertex index.
			// Convert (reverse) the triangle primitive drawing order from counter-clockwise (used by the Wavefront .obj specification) to clockwise (used by DirectX). *** Reverse   the Wavefront .obj order of geometric vertices ***
			OurIndices[(PrimitivesTotal * 3) - FaceIndexNo - 1] = strtol(tempchar, NULL, 10);
			// Wavefront .obj face index values start with 1, while the equivalent DirectX index buffer values start with 0.											*** Decrement the Wavefront .obj face index values ***
			--OurIndices[(PrimitivesTotal * 3) - FaceIndexNo - 1];

			// Prepare to parse the next statement.
			// Clear "tempchar" with the null-terminator.
			fill(tempchar, tempchar + MAX_ELEMENT_SIZE, '\0');
			// Reset "tempchar's" array element pointer.
			nCharCounter = 0;
			// Reset the flag indicator to save characters of a face element.
			ignoreFlag = 0;
			// Increment the number geometric vertex indices processed. Because it's incremented when the end of the statement is reached, its final value is one greater than the final value used.
			++FaceIndexNo;

			// The current "f " face element has been parsed. Get another statement from the while loop.
		}

		// Get another statement from the while loop.
	}

	// Close the Wavefront .obj file.
	obj.close();

	// End: 3. Parse the Wavefront.obj file : Assign each occurrence of each descriptive element in the Wavefront.obj file to its associated external global variable.

	// Terminate this function with a return code indicating success.
	return 0;
}