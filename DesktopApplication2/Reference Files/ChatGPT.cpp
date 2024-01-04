//***
//*** Save the following notes on parsing techniques.
//***
// To: AI: ChatGTP:
// Write a C++ external function that 
// 1. Reads a 3D object's description from a Wavefront OBJ file and 
// 2. Returns to the calling program all variables necessary to render the 3D object using DirectX 11.
//
// I edited it as follows:
// . Added comments.
// 
// After writing the following code, ChatGTP began to write the start of a C++ DirectX 11 rendering program (not shown, as not requested).
struct Vertex												// Structure type defining each element of 'vertices'.
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;
};

std::vector<Vertex> vertices;                               // 'vertices' is a 'vector' with each element having type 'Vertex'.
std::vector<unsigned int> indices;                          // 'indices'  is a 'vector' with each element having type 'unsigned int'.

bool LoadOBJ(const char* filePath, ID3D11Device* device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, int& vertexCount, int& indexCount)
{
	// Open the file
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return false;
	}

	// Read the file line by line
	std::string line;
	while (std::getline(file, line))
	{
		// Check if the line describes a vertex position. Initialize the appropriate 'vertex' member.
		if (line.substr(0, 2) == "v ")
		{
			Vertex vertex;                                  // 'vertex' has type 'Vertex', just like each element of 'vertices'. ||| Why is this declared in a 'while' loop? Won't this cause a redeclaration error?
			std::sscanf(line.c_str(), "v %f %f %f", &vertex.position.x, &vertex.position.y, &vertex.position.z); // Initialize 'vertex' member 'position' with geometric vertex data.
			vertices.push_back(vertex);                     // Add a new 'vertex' element to the end of 'vertices'.
		}

		// Check if the line describes a vertex normal*. Initialize the appropriate 'vertex' member.
		else if (line.substr(0, 3) == "vn ")
		{
			Vertex& vertex = vertices.back();				// Create a reference, 'vertex', to the last element of 'vertices'.
			std::sscanf(line.c_str(), "vn %f %f %f", &vertex.normal.x, &vertex.normal.y, &vertex.normal.z);		 // Initialize 'vertex' member 'normal' with vertex normal vector data.
		}

		// Check if the line describes a texture coordinate. Initialize the appropriate 'vertex' member.
		else if (line.substr(0, 3) == "vt ")
		{
			Vertex& vertex = vertices.back();				// Create a reference, 'vertex', to the last element of 'vertices'.
			std::sscanf(line.c_str(), "vt %f %f", &vertex.texCoord.x, &vertex.texCoord.y);						 // Initialize 'vertex' member texCoord with vertex texture coordinate data.
		}

		// Check if the line describes a triangle face.  Initialize 'indices'.
		else if (line.substr(0, 2) == "f ")
		{
			unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3]; // ||| Why is this declared in a 'while' loop? Won't this cause a redeclaration error? ||| This program appears to store all face element index types in these 3-element arrays and nowhere else.
			std::sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", // Initialize all three face element index types of a single face element.
				&vertexIndex[0], &texCoordIndex[0], &normalIndex[0],
				&vertexIndex[1], &texCoordIndex[1], &normalIndex[1],
				&vertexIndex[2], &texCoordIndex[2], &normalIndex[2]);
			for (int i = 0; i < 3; i++)
			{
				// Vertex texture coordinate conversion from Wavefront .obj to DirectX (flip the V coordinate).
				Vertex& vertex = vertices[vertexIndex[i] - 1]; // Create a reference, 'vertex', to the element of 'vertices' pointed to by vertexIndex[i] - 1 (this '-1' appears to be an interim index value conversion from Wavefront .obj to DirectX).
				vertex.texCoord = XMFLOAT2(1.0f - vertex.texCoord.x, vertex.texCoord.y); // Convert the vertex texture coordinate in the element of 'vertices' pointed to by vertexIndex[i] - 1.

				// ||| Only vertexIndex is added to 'indices'. Index value conversion from Wavefront .obj to DirectX.
				indices.push_back(vertexIndex[i] - 1);		// Add a new 'vertexIndex' element to the end of 'indices'.
			}
		}
	}
	// Close the file
	file.close();

	/*
	See '|||' above for my questions. Ask ChatGPT about these errors, but noting the compile errors perhaps rewrite from scratch using ideas from this program and Bing.AI.cpp.
	* :: 
	'Check if the line describes a vertex normal'
	In the given C++ code, the variable Vertex vertex; is used to create a new Vertex object and initialize its members with geometric vertex data from a line in an OBJ file. 
	Then the vertex object is added to the end of the vertices vector using the push_back() function.
	
	Later in the code, the reference Vertex& vertex = vertices.back(); is used to get a reference to the last element in the vertices vector. 
	This reference is then used to initialize the normal and texCoord members of the Vertex object with normal and texture coordinate data from later lines in the OBJ file.
	
	Using Vertex vertex; creates a new Vertex object each time it is called, while Vertex& vertex = vertices.back(); creates a reference to the last element of the vertices vector. 
	The latter is used to modify the already existing Vertex object in the vector, while the former creates a new object to be added to the vector.
	
	Overall, this code is reading data from an OBJ file and constructing a mesh from that data using a vector of vertices and a vector of indices. The Vertex struct holds information about the position, normal, and texture coordinates of each vertex in the mesh.
	*/