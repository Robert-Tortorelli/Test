#include <windows.h>
#include <vector>
#include <directxmath.h>

using std::vector;
using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

int WINAPI WinMain(HINSTANCE hInstance,						// The "handle to an instance" or "handle to a module." The operating system uses this value to identify the executable when it is loaded in memory.
	HINSTANCE hPrevInstance,								// This parameter has no meaning. It was used in 16-bit Windows, but is now always zero.
	LPSTR lpCmdLine,										// Contains the command line arguments, excluding the program name, as an ANSI string. To retrieve the entire command line, as a UNICODE string, use the GetCommandLine function.
	int nCmdShow)											// Indicates if the main program window will be minimized, maximized, or shown normally.
{
	// Without Statements #1.1 and #1.2, or Statement #2.1, this program would fail with a runtime out-of-bounds error when attempting to access an element of the vector v, because the vector would be empty.

	// Either use the following two statements:
	vector<XMFLOAT3> v;					     				// Statement #1.1: Create an empty vector of XMFLOAT3s.
	v.push_back(XMFLOAT3());								// Statement #1.2: Add one element to the vector before accessing it. Only v[0] is valid. v[1] through v[9] are not valid.

	// Or use the following one statement:
	//vector<XMFLOAT3> v(10);									// Statement #2.1: Create a vector with 10 default-constructed XMFLOAT3s. v[0] through v[9] are valid.

	// Access elements of the vector v:
	v[0].x = 1;												// Access an element at index 0. This is the first element in the vector.

	return 0;
}

// Question: As I want to use 'vector<>' to create dynamically allocated arrays without a predefined size, should I adapt Statements #1.1 and #1.2 to grow the vector arrays in objReader.cpp as needed, making sure to add elements to the vector array before accessing them?