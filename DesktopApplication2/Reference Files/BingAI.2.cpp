// BingAI.2.cpp is a copy of BingAI.cpp with all comments removed. This was done to prepare for an analysis of the code by GitHub Copilot Chat.
#include <string>
#include <vector>
void ReadObjFile(const char* filename, ObjData* data)
{
	std::ifstream file(filename);
	if (!file) return;

	std::string line;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;

	std::vector<int> indices;

	// Loop through each line
	while (std::getline(file, line))
	{
		// Skip empty or comment lines
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, {});

		if (tokens[0] == "v")
		{
			vertices.push_back(std::stof(tokens[1]));
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
		else if (tokens[0] == "vt")
		{
			texcoords.push_back(std::stof(tokens[1]));
			texcoords.push_back(std::stof(tokens[2]));
		}
		else if (tokens[0] == "f")
		{
			for (int i = 1; i < tokens.size(); i++)
			{
				std::istringstream iss2(tokens[i]);
				std::vector<std::string> subtokens(std::istream_iterator < std::getline(iss2), {});

				indices.push_back(std::stoi(subtokens[]) - 1);

				if (subtokens.size() >)
					indices.push_back(std::stoi(subtokens[]) - 1);

					if (subtokens.size() >)
						indices.push_back(std::stoi(subtokens[]) - 1);
			}