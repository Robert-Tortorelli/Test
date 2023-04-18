//***
//*** Save the following notes on parsing techniques.
//***
// To: AI: Bing AI:
// Write a C++ external function that 
// 1. Reads a 3D object's description from a Wavefront OBJ file and 
// 2. Returns to the calling program all variables necessary to render the 3D object using DirectX 11.
// 
// I edited it as follows:
// . Deleted extra carriage returns.
// . 'st' -> 'std'
// . 'push_back(std::stoi(subtokens[]) - )' -> 
// . 'push_back(std::stoi(subtokens[]) - 1)' // Is "-1" intended?
// . Added '#include's
// . Added comments.
//
// This is the part of the answer that concerns parsing the Wavefront OBJ file.
#include <string>
#include <vector>
void ReadObjFile(const char* filename, ObjData* data)
{
	// Open the file                                        // ||| Invalid
	std::ifstream file(filename);
	if (!file) return;

	// Declare some variables
	std::string line;
	std::vector<float> vertices;                            // Each vertex normal vector, vertex texture coordinate, geometric vertex is stored in a separate variable, which is not what my objRenderer needs (it only uses the OurVertices variable).
	std::vector<float> normals;                             //   "
	std::vector<float> texcoords;                           //   "
	/*
	Vertex normal vector indices and vertex texture coordinate indices are used in objReader solely to determine where to store vertex normal vectors and where to store vertex texture coordinates in the OurVertices variable.
	They are stored in the OurVertices variable in the same array element as their associated geometric vertices.
	*/
	std::vector<int> indices;                               // All three face element indices are stored in this variable, which is not what my objRenderer needs (it needs ).

	// Loop through each line
	while (std::getline(file, line))						// 'line' is a string containing a line of the file 'file'.
	{
		// Skip empty or comment lines
		if (line.empty() || line[0] == '#') continue;

		// Split the line by spaces
		/*
		'std::istringstream iss(line)' declares an 'std::istringstream' input string stream object that reads data from the string 'line' as if it were coming from an input stream like std::cin. You can use it to read data from a string into variables of different types.
		
		'std::istream_iterator' is an input stream input iterator class template that is used to read data from an input stream and iterate over its contents.
		'std::istream_iterator<std::string>{iss}, {})' creates an `std::istream_iterator` input stream input iterator object that iterates over strings read from the `std::istringstream` input string stream object `iss`.
		The `{}` at the end of the line is an empty brace-enclosed initializer list (empty initializer) that is used to create an end-of-stream iterator, i.e., it indicates that each character in the input stream should be treated as a separate token.
		'std::istream_iterator<std::string>{iss}, " ")' indicates that the space character is the end-of-stream iterator, i.e., it indicates that each space character in the input stream delimits a separate token.
		
		'std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, {})' is a constructor that uses an iterator to declare and define 'tokens', a vector of strings containing all the tokens in the string 'line'.
		
		'std::vector<std::string> tokens{"a","b"}'                                     is a constructor being used to            declare and define 'tokens', a vector of strings with two elements “a” and “b”.
		*/
		std::istringstream iss(line);
		std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, {}); // 'tokens' is a vector of type string. 

		// Check the first token
		if (tokens[0] == "v") // Vertex position
		{
			// Parse and store x, y, z coordinates
			vertices.push_back(std::stof(tokens[1])); // ||| Is this code correct? If "each character in the input stream should be treated as a separate token" then x, y and z will not be parsed correctly.
			vertices.push_back(std::stof(tokens[2]));
			vertices.push_back(std::stof(tokens[3]));
		}
		else if (tokens[0] == "vn") // Vertex normal
		{
			// Parse and store x, y, z components
			normals.push_back(std::stof(tokens[1]));
			normals.push_back(std::stof(tokens[2]));
			normals.push_back(std::stof(tokens[3]));
		}
		else if (tokens[0] == "vt") // Vertex texture coordinate
		{
			// Parse and store u, v components
			texcoords.push_back(std::stof(tokens[1]));
			texcoords.push_back(std::stof(tokens[2]));
		}
		else if (tokens[0] == "f") // Face
		{
			// Loop through each vertex index group
			for (int i = 1; i < tokens.size(); i++)
			{
				// Split by slashes
				std::istringstream iss2(tokens[i]);
				std::vector<std::string> subtokens(std::istream_iterator < std::getline(iss2), {});

				// Parse and store position index - note: OBJ indices start from one but we want zero - based indices
				indices.push_back(std::stoi(subtokens[]) - 1);

				if (subtokens.size() >)
					// Texture coordinate index - optional
					indices.push_back(std::stoi(subtokens[]) - 1);

					if (subtokens.size() >)
						// Normal index - optional
						indices.push_back(std::stoi(subtokens[]) - 1);
			}