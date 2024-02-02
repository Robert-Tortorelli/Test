// Reading and parsing a Wavefront .obj file in C++ involves opening the file, reading it line by line, and then parsing each line based on the Wavefront .obj file format. Here's a basic example of how you might do this:

#include <fstream>
#include <sstream>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

void readObjFile(const std::string& filename) {
    std::ifstream objFile(filename);
    if (!objFile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::vector<XMFLOAT3> vertices;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texCoords;

    std::string line;
    while (std::getline(objFile, line)) {
        std::istringstream lineStream(line);				// Declaring lineStream inside the loop ensures that it is reset and ready to process the next line of the file in each iteration of the loop.
        std::string type;									// Wavefront .obj file statement type: v, vn, vt, f, etc.
															// Since the value of type is specific to each line, it makes sense to declare it inside the loop.
															// Declaring type inside the loop ensures that it is reset at the start of each iteration, which is generally what you want when reading a new line from the file.
															// If type were declared outside the loop, its value from the previous iteration would persist, which could lead to bugs if not handled correctly.
															// In general, it's a good practice to limit the scope of variables as much as possible to prevent unintended side effects.
															// In this case, since type is only used within the loop, it's best to declare it there.
        lineStream >> type;									// The >> operator extracts data from the lineStream object and stores it in the variable type.

/*
Contrast using std::sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z); vs. using lineStream >> vertex.x >> vertex.y >> vertex.z;
(See ChatGPT.cpp for an example of sscanf())

Both `std::sscanf` and `std::istringstream` can be used to parse strings in C++, but they have some differences:

1. **Ease of Use:** `std::istringstream` is generally easier to use and more C++-like. It uses the stream extraction operator (`>>`), which automatically handles whitespace and can be chained to read multiple values. `std::sscanf`, on the other hand, requires a format string and a list of output variables, which can be more error-prone.

2. **Type Safety:** `std::istringstream` is more type-safe than `std::sscanf`. With `std::istringstream`, the compiler will give an error if you try to read a value into a variable of the wrong type. With `std::sscanf`, you could accidentally read a value into a variable of the wrong type and not realize it until runtime.

3. **Performance:** `std::sscanf` can be faster than `std::istringstream` in some cases because it doesn't have the overhead of creating a stream object. However, the difference is usually small and unlikely to be noticeable unless you're parsing a very large amount of data.

4. **Flexibility:** `std::istringstream` is more flexible than `std::sscanf`. With `std::istringstream`, you can easily read values of any type that has an overload for the stream extraction operator. With `std::sscanf`, you're limited to the types and formats supported by the format specifiers.

In your case, both `std::sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);` and `lineStream >> vertex.x >> vertex.y >> vertex.z;` would work, but `lineStream >> vertex.x >> vertex.y >> vertex.z;` is more idiomatic C++, easier to use, and more type-safe.
*/

        if (type == "v") {
            XMFLOAT3 vertex;                                // Equivalent to my v variable.
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);						// Corresponds to populating my OurVertices variable with my v variable.
        } else if (type == "vn") {
            XMFLOAT3 normal;                                // Equivalent to my vn variable.
            lineStream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);						// Corresponds to populating my OurVertices variable with my vn variable.
        } else if (type == "vt") {
            XMFLOAT2 texCoord;                              // Equivalent to my vt variable.
            lineStream >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);					// Corresponds to populating my OurVertices variable with my vt variable.
        } else if (type == "f") {
            // Parse face data here
            // Note: This is a simplified example and assumes that faces are defined as "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
            // You may need to adjust this to suit the specific format of your .obj files
        }
    }
}

/*
std::istringstream lineStream(line);
The lineStream variable is declared inside the while loop because it is used to process each line of the input file individually.
In this code snippet, the while loop reads each line of the objFile using std::getline(objFile, line). Then, the lineStream object is created using the current line as input. By declaring lineStream inside the loop, a new lineStream object is created for each line of the file, allowing us to process the data on a line-by-line basis.
Declaring lineStream inside the loop ensures that it is reset and ready to process the next line of the file in each iteration of the loop. This approach is useful when you need to perform operations on each line independently, without carrying over any state or data from the previous lines.

lineStream >> type;
This code opens the .obj file and reads it line by line. It then parses each line based on the first word in the line (the "type"). If the type is "v", it reads the next three numbers as a vertex and adds it to the `vertices` vector. If the type is "vn", it reads the next three numbers as a normal and adds it to the `normals` vector. If the type is "vt", it reads the next two numbers as a texture coordinate and adds it to the `texCoords` vector. If the type is "f", it reads the face data. This is a simplified example and assumes that faces are defined in a specific way. You may need to adjust this to suit the specific format of your .obj files.

The code is using the >> operator to extract data from a lineStream object and store it in the variable type.
The >> operator is known as the extraction operator in C++. It is commonly used to read data from input streams, such as std::cin or in this case, lineStream.

In this specific line of code, it is assumed that lineStream is an input stream object, such as an instance of std::istringstream, which allows you to read data from a string. The >> operator is used to extract the next value from the stream and store it in the variable type.
The type of type should match the type of data being read from the stream. For example, if type is an integer, the code would look like this:

After this line of code executes, the variable type will contain the value that was read from the lineStream object.

Declaring a variable inside a loop in C++ is not generally considered inefficient. The cost of declaring a variable is very small, and modern compilers are very good at optimizing your code. 
When you declare a variable inside a loop, it's created and destroyed on each iteration. However, this doesn't mean that memory is allocated and deallocated on each iteration. For simple types like `int`, `float`, `std::string`, etc., the memory is usually allocated on the stack, which is very fast.
For more complex types that allocate memory on the heap (like `std::vector`, `std::map`, etc.), the cost of declaring the variable inside the loop could be higher because these types may allocate and deallocate memory when they are created and destroyed. But in your case, `std::string` and `std::istringstream` are not likely to cause a performance issue.
In general, it's a good practice to limit the scope of a variable as much as possible. If a variable is only used inside a loop, it's best to declare it inside the loop. This makes your code easier to understand and less prone to bugs. If you're concerned about performance, it's usually better to focus on algorithmic efficiency rather than micro-optimizations like this. If you find that your program is running slower than expected, use a profiler to find the bottlenecks before trying to optimize your code.

Processing a large Wavefront .obj file efficiently in C++ involves a few strategies:
1. Use Buffered I/O:
   The standard C++ I/O streams are buffered by default, which means they read data from the disk in large chunks, rather than one character at a time. This can significantly improve the speed of reading a large file.
2. *Minimize Dynamic Memory Allocation:
   If you're storing the vertices, normals, and texture coordinates in `std::vector` objects, consider reserving a large amount of memory upfront with the `reserve` function if you have an estimate of the number of each element. This can prevent costly reallocations as the vectors grow.
3. Use Multi-Threading:
   If the file is very large, you might benefit from using multiple threads to process different parts of the file simultaneously. This is a more advanced technique and requires careful coordination between threads to avoid data races.
4. Process Data in Chunks:
   Instead of reading the entire file into memory, consider processing the file in chunks. This can be especially beneficial if the file is too large to fit into memory.

*/