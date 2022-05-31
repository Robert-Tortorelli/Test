// See HERE
// DirectX 11 Win32 Desktop: Direct3D: Moving to 3D: Lesson 3: Simple Modeling
// Source.E2.cpp
//
// Comment the source code that relates to the stages of the pipeline. This will indicate how the graphics card is used to perform most of the graphics processing.
// For example, for IASetVertexBuffers use the comment "Bind an array of vertex buffers to the input-assembler stage."
//
// This program uses shaders from a shaders.hlsl file.
// "I chose the Hypercraft model for this demo. It rotates around much like a car advertisement, only it looks somewhat more polygonal than an actual car."
// Questions: // Lesson 3D 3
//
// 1. What's the difference between a vertex buffer and an index buffer.
//    Vertex buffers allow the video card to store vertices in local video memory, which can be rendered quickly by the GPU. 
//    An index buffer lets us reuse vertices that we have already defined in the vertex buffer. For example, a square is formed by two triangles. These two triangles share two vertices. 
//    Without an index buffer, we would have to specify six separate vertices even though two of the vertices are shared. 
//    With an index buffer, we would only have to specify four vertices and use indices to access them. http://www.chadvernon.com/blog/resources/directx9/vertex-and-index-buffers/
//
// 2. What is the size of an index in the index buffer?
//    It's an unsigned long; i.e., a DWORD.
//
// Questions: // Lesson 3D 1
//
// 1. Why does the triangle disappear when it turns around? (answer is in lesson 1)
//    There is more than one "lesson 1" in this set of lessons. The answer is in the lesson "Direct3D: Getting Started: Lesson 1: Understanding Graphics Concepts", under "A Primitive Quirk".
//    Note how you can show both sides of the triangle, which would be useful when going inside a 3D object and viewing its interior (which would otherwise be invisible).
//    
// 2. What is the purpose of the projection transform?
//    The projection transform converts model vertices to screen coordinates.
//    Projection transformation can be considered similar to setting up a camera lens, and involves:
//    a. Field of View
//       In 3D graphics, the field of view is defined by setting the amount of radians allowed (vertically). The normal amount for this is 0.78539 (which is pi/4 radians, or 45 degrees).
//    b. View-Plane Clipping
//       View-plane clipping omits the parts of an image that are unnecessary to draw; e.g., parts too far to see because of fog or the horizon.
//       Direct3D asks for a near view-plane and a far view-plane, and only draws the graphics that are between them; i.e., in the viewing frustum.
//    c. It's defined by a single matrix (matProjection), using a single DirectX function.
//       However, it's probably the most complex type of transformation, and the underlying math performed by the DirectX function is complicated.
//
// 3. What is the purpose of the view transform?
//    The view transform converts model vertices to view coordinates.
//    View transformation can be considered similar to setting up a virtual camera, and involves:
//    a. Position of the camera
//    b. Location the camera is looking at
//    c. Orientation of the camera
//       This is the direction of "up" for the camera; i.e., the direction pointing to the top of the screen. Usually, game programmers use the y-axis as the "up" direction.
//       To orient the camera this way, specify (0, 1, 0), or 1 on the y axis and 0 on the x and z axes; i.e., (0.0f, 1.0f, 0.0f).
//    d. It's defined by a single matrix (matView), using a single DirectX function.
//
// x. What is the purpose of the world transform? (I added this question for completeness)
//    The world transform converts model vertices to world coordinates.
//    In other words, it places a model in a world at an exact point defined by coordinates, and involves:
//    a. Translation (moving)
//       It's defined by a single matrix (matTranslate), using a single DirectX function.
//    b. Rotation (*** In this program, the lesson's program, only rotation is performed. The exercises use more. ***)
//       It's defined by one to three matrices (matRotateX, matRotateY and / or matRotateZ), using one to three DirectX functions for x, y, and / or z axis rotation.
//    c. Scaling
//       It's defined by a single matrix (matScale), using a single DirectX function.
//    d. World Matrix (matWorld) Examples
//       matWorld = matScale x matRotateX
//       matWorld = matRotateX x matRotateY x matRotateZ x matScale x matTranslate (translate last, which is most commonly used)
//       matWorld = matTranslate x matRotateX x matRotateY x matRotateZ x matScale (translate first, providing a different result because here translation is offseting the object from the origin (0, 0, 0) before rotation takes place relative to the origin)
//
// 4. What is done with the final matrix to convert a 3D location into screen space?
//    Multiply each vertex by the final matrix (matFinal): matFinal = matWorld x matView x matProjection
//    For efficiency, this multiplication is performed in the vertex shader.
//
// Exercises: (all exercises were done in this program; no separate program was created in the HW folder)
//
// 1. Make a triangular pyramid.
// 2. Draw 20 copies of your triangular pyramid on the screen at once (in different positions).
//    See comments labeled "// Lesson 3D 3 Exercise 2".
// 3. Design and build your own mini-spaceship.

// include the basic windows header file and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include <d3dcompiler.h> // Needed for D3DCompileFromFile

// Other header files // Lesson 3D 3 Exercise 2
#include <random> // Random number generation

// DirectXMath: // Lesson 3D 1
// "The DirectXMath library is designed for C++ developers working on games and DirectX graphics in Windows Store apps and traditional desktop apps for Windows 8 and later."
//   See http://msdn.microsoft.com/en-us/library/windows/desktop/hh437833(v=vs.85).aspx

// DirectXMath // Lesson 3D 1
// DirectXMath replaced D3DXMath, which is deprecated for Windows 8. D3DXMath is used in the associated DirectXTutorial.com lesson. I manually replaced the following 
// D3DXMath types and functions used in the associated DirectXTutorial.com lesson with their DirectXMath equivalents: D3DXMATRIX, D3DXMatrixRotationY, 
// D3DXMatrixLookAtLH, D3DXVECTOR3, D3DXMatrixPerspectiveFovLH, and D3DXToRadian. For example, D3DXMatrixRotationY was replaced by XMMatrixRotationY.
//
// To use DirectXMath Library functions, include the DirectXMath.h, DirectXPackedVector.h, DirectXColors.h, and/or DirectXCollision.h headers.
#include <DirectXMath.h>
// Optional but useful using statement for DirectXMath types; e.g., type XMMATRIX.
// "For headers, it is not considered ‘best practice’ to add using statements. Instead, add fully qualified namespaces:" For example, DirectX::"namespace". I have NOT done this.
// See http://msdn.microsoft.com/en-us/library/windows/desktop/ee418730(v=vs.85).aspx // Lesson 3D 1
using namespace DirectX;

// Direct3D Library files
#pragma comment (lib, "d3d11.lib")     // Direct3D DirectX 11 Library
#pragma comment(lib,"d3dcompiler.lib") // Direct3D Compiler Library (needed by D3DCompileFromFile, which compiles shaders)

// define the screen resolution of the client area
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer // Lesson 3D 2
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer // Lesson 4
ID3D11Buffer *pIBuffer;                // the pointer to the index buffer // Lesson 3D 3

// various buffer structs
// a struct to define a single vertex
struct VERTEX{FLOAT X, Y, Z; float Color[4];};
// a C++ struct to match the constant buffer in HLSL
// XMMATRIX matFinal; // matFinal could be defined here, but is instead defined in function RenderFrame. // Lesson 3D 1

// Function Prototypes
void InitD3D(HWND hWnd);               // This function initializes and prepares Direct3D for use.
void InitPipeline(void);               // This function loads and prepares the shaders.
void InitGraphics(void);               // This function creates the shape to render.
void RenderFrame(void);                // This function renders a single frame.
void CleanD3D(void);                   // This function cleans up Direct3D and COM.

// The WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    // the handle for the window, filled by a function (i.e., function CreateWindowEx)
    HWND hWnd;
    // The WNDCLASSEX struct holds information for the window class
	// "By the way, the 'EX' is there to indicate that this is the extended version of the struct WNDCLASS, which is essentially the same, 
	// but without a couple extra things we won't get into here. The same goes for the CreateWindowEx() and RegisterClassEx() functions. Each have their older, 
	// smaller counterpart without the 'Ex', which have less options."
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the window class struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;    // Use when the window is not fullscreen (via scd.Windowed = TRUE)
	//   wc.hbrBackground
    //   When scd.Windowed = FALSE (i.e., fullscreen), modify the window to leave the background color untouched, so it won't be visible as a window for a 
	//   second or two before the program starts in fullscreen (theyby making the game look professional)*.
	//   To leave the window's background color untouched, comment out wc.hbrBackground, which is used to set the background color.
	//   * While I sometimes observed this effect in "Lesson 3: Rendering Frames", it was never observed in later lessons.
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // AdjustWindowRect
	// Calculate the appropriate window rectangle size needed by the client rectangle. The client rectangle is within the window rectangle.
	// (the window rectangle also contains the window border and, if present, the menu).
	// AdjustWindowRect calculates the required size of the window rectangle, based on the desired client rectangle size. 
	// The window rectangle can then be passed to the CreateWindowEx function to create a window whose client area is the desired size.
	// AdjustWindowRect will take:
	//   1) A pointer to a RECT structure (here defined as wr) that contains the coordinates of the top-left (here 0, 0; the true top-left corner will be set by CreateWindowEx) and 
	//      bottom-right (here SCREEN_WIDTH, SCREEN_HEIGHT) corners of the desired client area. When the function returns, the RECT structure contains the coordinates of the top-left and 
	//      bottom-right corners of the window to accommodate the desired client area.
	//   2) The window style
	//   3) Whether (TRUE) or not (FALSE) we are using menus
    // Note AdjustWindowRect is used and not AdjustWindowRectEx. Why? (By contrast, see WNDCLASSEX, etc.)
	RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",                  // name of the window class
                          L"Our First Direct3D Program",    // title of the window
                          WS_OVERLAPPEDWINDOW,              // window style
                          300,                              // x-position of the window's true top-left corner, in screen coordinates
                          300,                              // y-position of the window's true top-left corner, in screen coordinates
                          wr.right - wr.left,               // width of the window, adjusted by AdjustWindowRect
                          wr.bottom - wr.top,               // height of the window, adjusted by AdjustWindowRect
                          NULL,                             // we have no parent window, NULL
                          NULL,                             // we aren't using menus, NULL
                          hInstance,                        // A handle to the instance of the module (WINAPI) to be associated with the window.
                          NULL);                            // used with multiple windows, NULL

	// display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    InitD3D(hWnd); // This function initializes and prepares Direct3D for use.

    // enter the main loop:

    // this struct holds Windows event messages
	// Does "= {0}" set any struct to null? // Lesson 3D 1
    MSG msg = {0};

    // Enter the infinite message loop
    while(TRUE)
    {
        // Check to see if any messages are waiting in the queue
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function, the main message handler for this program
            DispatchMessage(&msg);

            // check to see if it's time to quit; i.e., the window is closed
            if(msg.message == WM_QUIT)
                break;
        }
        else
        {

            // Run game code here

            RenderFrame(); // This function renders a single frame.
        }
    }

    CleanD3D(); // This function cleans up Direct3D and COM.

    // Return the wParam parameter of the WM_QUIT message to Windows (see PostQuitMessage).
    return msg.wParam;
}

// WindowProc is the main message handler for this program (see wc.lpfnWndProc = WindowProc)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
				// Post a WM_QUIT message to the thread's message queue.
                PostQuitMessage(0); // (x), where x is the application exit code. This value is used as the wParam parameter of the WM_QUIT message.
                return 0;
            } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// This function initializes and prepares Direct3D for use.
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width (needed when going Fullscreen)
    scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height (needed when going Fullscreen)
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed (TRUE)/Fullscreen (FALSE) initial mode (FALSE to start Fullscreen). Changed to TRUE in Lesson 5
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching (via Alt-Enter) (needed when going Fullscreen)

    // create a device, device context and swap chain using the information in the scd struct

	D3D_FEATURE_LEVEL pFeatureLevelsIn[] = // A pointer to an array of D3D_FEATURE_LEVELs, which determine the order of feature levels to attempt to create. See the explanation of pFeatureLevelOut. // Lesson 3D 1
	{
		D3D_FEATURE_LEVEL_11_1,            // If NULL is used instead of featureLevels (in D3D11CreateDeviceAndSwapChain's 5th parameter) then a default list which omits D3D_FEATURE_LEVEL_11_1 is used. 
		                                   // That's okay, as I don't actually need D3D_FEATURE_LEVEL_11_1 (DirectX 11.1) features in this program. However I'm including it here for completeness.
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL pFeatureLevelOut; // D3D11CreateDeviceAndSwapChain sets this to the feature level of the device created. It reflects the capability of the graphics hardware 
	                                    // running this program. See the explanation of pFeatureLevelOut in D3D11CreateDeviceAndSwapChain. // Lesson 3D 1

	D3D11CreateDeviceAndSwapChain(NULL, // A pointer to the video adapter to use when creating a device. Pass NULL to use the default adapter.
		// D3D_DRIVER_TYPE_HARDWARE // Lesson 3D 1
		// Use D3D_DRIVER_TYPE_REFERENCE or D3D_DRIVER_TYPE_WARP (rather than D3D_DRIVER_TYPE_HARDWARE) if hardware is unavailable to support the Direct3D features used by this program.
		// D3D_DRIVER_TYPE_REFERENCE supports every Direct3D feature level but is very slow.
		// D3D_DRIVER_TYPE_WARP supports Direct3D feature levels with a high performance software implementation.
		// D3D_DRIVER_TYPE_HARDWARE failed to work when Direct3D 11.0 and 11.1 shader compiler targets were specified in D3DCompileFromFile, 
		// instead of the correct Direct3D 10.1 shader compiler targets appropriate to the ATI Radeon 4850 x2 I'm using to run this program.
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,                          // An optional handle to a DLL that implements a software rasterizer: non-NULL when using D3D_DRIVER_TYPE_SOFTWARE and NULL otherwise.
		NULL,                          // Optionally set debug and Direct2D compatibility flags.
		pFeatureLevelsIn,              // A pointer to an array of D3D_FEATURE_LEVELs. It's explained elsewhere in this program. // Originally NULL // Lesson 3D 1
		ARRAYSIZE(pFeatureLevelsIn),   // This is the number of elements in pFeatureLevelsIn // Originally NULL // Lesson 3D 1
		D3D11_SDK_VERSION,             // The SDK version; use D3D11_SDK_VERSION.
		&scd,                          // A pointer to a swap chain description (see struct DXGI_SWAP_CHAIN_DESC) that contains initialization parameters for the swap chain.
		&swapchain,                    // Returns the address of a pointer to the IDXGISwapChain object that represents the swap chain used for rendering.
		&dev,                          // Returns the address of a pointer to an ID3D11Device object that represents the device created.
		// pFeatureLevelOut // Lesson 3D 1
		// http://msdn.microsoft.com/en-us/library/windows/desktop/ff476083(v=vs.85).aspx:
		// Parameter pFeatureLevelOut "Returns a pointer to a D3D_FEATURE_LEVEL, which represents the first (highest (RJT)) element in an array of feature levels supported by the device."
		// Thus parameter pFeatureLevelOut returns D3D_FEATURE_LEVEL_10_1 for an ATI Radeon 4850 x2, as it's a DirectX 10.1 graphics card.
		// D3D_FEATURE_LEVEL_10_1 tells DirectX 11 how it should adjust to this graphics card; i.e., downgrade from DirectX 11 to DirectX 10.1.
		// (When D3D_DRIVER_TYPE_WARP is used, pFeatureLevelOut returns D3D_FEATURE_LEVEL_11_1)
		//
		// NULL is sufficient if the graphics card returns feature level D3D_FEATURE_LEVEL_9_1 or higher and the appropriate shader compiler target value (used in function D3DCompileFromFile) is known and fixed. 
		// Otherwise use and inspect pFeatureLevelOut to (1) detect a feature level lower than D3D_FEATURE_LEVEL_9_1; or to (2) determine the shader compiler target value appropriate for function D3DCompileFromFile:
		// (1) Inform the user they have an insufficient graphics card via a message written using DirectX 2D. DirectX 2D is supported by most (all?) graphics cards.
		// (2) The returned feature level must be used to determine the appropriate shader compiler target for function D3DCompileFromFile (see the coding of that function for more information).
		&pFeatureLevelOut,             // Returns a pointer to a D3D_FEATURE_LEVEL // Originally NULL // Lesson 3D 1
		&devcon);                      // Returns the address of a pointer to an ID3D11DeviceContext object that represents the device context.

	// Create the depth/stencil: Start // Lesson 3D 2
	
	// Create the depth/stencil buffer texture object.
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = SCREEN_WIDTH;
	texd.Height = SCREEN_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *pDepthBuffer;
	dev->CreateTexture2D(&texd, NULL, &pDepthBuffer); // Create the depth/stencil buffer texture object (pDepthBuffer). &pDepthBuffer is the address of a pointer (pDepthBuffer) to the ID3D11Texture2D interface for the buffer texture object created, in this case a depth/stencil buffer texture object as per texd.BindFlags = D3D11_BIND_DEPTH_STENCIL.

	// Create the depth/stencil view object.
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer); // Create the depth/stencil view object (zbuffer). &zbuffer is the address of a pointer (zbuffer) to the ID3D11DepthStencilView interface for the object created, in this case a depth/stencil view object (a z-buffer).
	pDepthBuffer->Release();

	// Create the depth/stencil: End // Lesson 3D 2

	// Create the back buffer texture: Start

    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer); // Get the address of the back buffer interface and store it at pBackBuffer. &pBackBuffer is the address of a pointer (pBackBuffer) to the ID3D11Texture2D interface for the buffer (texture) created, in this case a back buffer.

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer); // Create the render target view object (backbuffer). &backbuffer is the address of a pointer (backbuffer) to the ID3D11RenderTargetView interface for the object created, in this case a back buffer.
    pBackBuffer->Release();

    // Set the render target (backbuffer) as the back buffer, and set the depth/stencil view object (zbuffer). // Lesson 3D 2
	// Set the Output-Merger Stage to write to the depth buffer (the depth/stencil view object (zbuffer)).
	devcon->OMSetRenderTargets(1, &backbuffer, zbuffer); // Specify zbuffer as the pointer to a ID3D11DepthStencilView that represents the depth-stencil view to bind to the device. It was originally NULL, indicating the depth-stencil state is not bound. // Lesson 3D 2

	// Create the back buffer: End

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0;           // The closest an object can be on the depth buffer is 0.0. // Lesson 3D 2
	viewport.MaxDepth = 1;           // The farthest an object can be on the depth buffer is 1.0. // Lesson 3D 2

    devcon->RSSetViewports(1, &viewport); // Bind an array of viewports to the rasterizer stage of the pipeline.

    InitPipeline(); // This function loads and prepares the shaders.
    InitGraphics(); // This function creates the shape to render.
}

// This function loads and prepares the shaders.
void InitPipeline()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS; // Why is ID3D10Blob and not ID3D11Blob used? // Lesson 3D 1
	// D3DCompileFromFile Shader Compiler Targets // Lesson 3D 1
	// Direct3D 10.1 shader compiler targets specified: "vs_4_1" and "ps_4_1". More generally, the returned feature level of the device created by function D3D11CreateDeviceAndSwapChain should be used to determine and here assign the appropriate 
	// shader compiler target values (see the coding of that function for more information).
	// See 'Specifying Compiler Targets' at http://msdn.microsoft.com/en-us/library/windows/desktop/jj215820(v=vs.85).aspx
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_4_1", 0, 0, &VS, 0); // Lesson 3D 1
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_4_1", 0, 0, &PS, 0); // Lesson 3D 1

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// Create the input layout object.
	// "You may be wondering how the GPU is capable of reading our vertices, when we placed them in our own, self-created struct. How can it know that we placed location before color? How can it know we didn't mean something else? The answer is the input layout."
	// See "DirectX 11 Win32 Desktop: Direct3D: Lesson 5: Drawing a Triangle".
	// The fifth value indicates how many bytes, into the VERTEX struct OurVertices, the input element (position or color) is. 
	// This is called the offset. As you can see, position has an offset of 0 and the color has an offset of 12. 
	// This means the position starts 0 bytes into the struct and the color starts 12 bytes into the struct. 
	// Actually, you can just put D3D11_APPEND_ALIGNED_ELEMENT and it will figure it out for you.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },   // Define the position input element of the VERTEX struct OurVertices.
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },  // Define the color    input element of the VERTEX struct OurVertices.
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);      // Create the input layout object pointed to by pLayout.
	devcon->IASetInputLayout(pLayout);                                                          // Set    the input layout object pointed to by pLayout.

	// Create a constant buffer object. // Lesson 4
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;                // Create a resource that requires read and write access by the GPU. This is likely to be the most common usage choice.
	bd.ByteWidth = 64;                             // The constant buffer object must be a multiple of 16 bytes, regardless of the size of the C++ struct that matches it (in this program it's matFinal, which as it happens is 4 x 16 = 64 bytes).
	                                               // This is required ("the size of a constant buffer is always a multiple of 16 bytes. This is because constants are sent to the GPU in packs of 16 bytes"). // Lesson 5
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;     // Use the buffer object as a constant buffer.

	dev->CreateBuffer(&bd, NULL, &pCBuffer);       // Create the constant buffer object (pCBuffer). &pCBuffer is the address of a pointer (pCBuffer) to the ID3D11Buffer interface for the buffer object created, in this case a constant buffer as per bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER. // Lesson 4

	// Set the constant buffer object. // Lesson 4.
	devcon->VSSetConstantBuffers(0, 1, &pCBuffer); // This same statement can be modified to "set" two constant buffers: devcon->VSSetConstantBuffers(0, 2, &pCBuffer); See Lesson 5 for further elaboration, and an explanation of why this can be efficient. // Lesson 5
}

// This function creates the shape to render.
void InitGraphics()
{
	// Create a Square Pyramid using the VERTEX struct (Create vertices to represent the corners of the Square Pyramid) // Lesson 3D 3 Exercise 1
	VERTEX OurVertices[] =
	{
	 // {     position         {    color               } } color has an offset of 12 as it starts 12 bytes into the VERTEX struct (a float is 4 bytes and position is comprised of 3 floats; color follows position).
	 // {     x,     y,     z, {    r,    g,    b,    a } }
		// base
		{ -1.0f, -1.0f,  1.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{  1.0f, -1.0f,  1.0f, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ -1.0f, -1.0f, -1.0f, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{  1.0f, -1.0f, -1.0f, { 0.0f, 1.0f, 1.0f, 1.0f } },

		// top
		{  0.0f,  1.0f,  0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } }

	};

	// Create the vertex buffer object.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                                  // Create a resource that is accessible by both the GPU (read only) and the CPU (write only). A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                      // Allow write access by the CPU. The resource is to be mappable so that the CPU can change its contents. Resources created with this flag cannot be set as outputs of the pipeline and must be created with either dynamic or staging usage (see D3D11_USAGE).
	bd.ByteWidth = sizeof(VERTEX)* 5;                                // The vertex buffer object's size is the size of the VERTEX struct * 5 // Lesson 3D 3 Exercise 1
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                         // Use the buffer object as a vertex buffer.

	dev->CreateBuffer(&bd, NULL, &pVBuffer);                         // Create the vertex buffer object (pVBuffer). &pVBuffer is the address of a pointer (pVBuffer) to the ID3D11Buffer interface for the buffer object created, in this case a vertex buffer as per bd.BindFlags = D3D11_BIND_VERTEX_BUFFER.

	// Copy the vertices into the vertex buffer.
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // Map the vertex buffer. To update a dynamic resource, use a Map method.
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));              // Copy the data.
	devcon->Unmap(pVBuffer, NULL);                                   // Unmap the vertex buffer.

	// Create the index buffer: Start // Lesson 3D 3 Exercise 1

	// create the index buffer's indices out of DWORDs
	DWORD OurIndices[] =
	{
		0, 2, 1,    // base
		1, 2, 3,
		0, 1, 4,    // sides
		1, 3, 4,
		3, 2, 4,
		2, 0, 4,
	};

	// create the index buffer
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DWORD)* 18;                                   // The index buffer object's size is the size of a DWORD struct * 18 // Lesson 3D 3 Exercise 1
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	dev->CreateBuffer(&bd, NULL, &pIBuffer);

	devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurIndices, sizeof(OurIndices));                   // copy the data
	devcon->Unmap(pIBuffer, NULL);

	// Create the index buffer: End // Lesson 3D 3 Exercise 1
}

// This function renders a single frame.
void RenderFrame(void)
{
	// The following statements, through "devcon->UpdateSubresource", contain changes made in "Lesson 1: 3D Transformations" (Lesson 3D 1). // Lesson 3D 1. See that lesson for the exact changes made, as subsequent lessons made further changes to these statements. // Lesson 3D 2
	// See my comments on DirectXMath at the beginning of this program.

	// Matrix Definitions
	// matFinal is a C++ struct matching the constant buffer in HLSL. It contains data that will be copied to the HLSL shader constant buffer variable matFinal (they do not have to have the same name).
	XMMATRIX matRotate, matWorld, matView, matProjection, matFinal; // Removed matTranslate[4] and changed other select matrices to non-arrays. // Lesson 3D 3
	XMMATRIX matTranslate; // Lesson 3D 3 Exercise 2

	// Create a rotation matrix. // Unlike in the prior lesson, no "reverse side" triangles are calculated; instead, they are explicitly defined. Only one object is drawn. // Lesson 3D 3
	static float Time = 0.0f; Time += 0.001f;
	matRotate = XMMatrixRotationY(Time);
	// Create a world matrix. // Lesson 3D 2
	matWorld = matRotate;

	// Create a view matrix.
	// See http://vplab.snu.ac.kr/lectures/11-1/comp_appl/03_Basic_Direct3D_Programming.pdf
	// See http://msdn.microsoft.com/en-us/library/windows/apps/hh780565.aspx
	XMVECTOR eye = XMVectorSet(0.0f, 9.0f, 24.0f, 0.0f); // Lesson 3D 3
	XMVECTOR focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	matView = XMMatrixLookAtLH(eye, focus, up);

	// Create a projection matrix.
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45),                     // field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,                                       // near view-plane
		100.0f);                                    // far view-plane

	// Create the final transform matrix. // Lesson 3D 3
	matFinal = matWorld * matView * matProjection;

	// Clear the back buffer to a deep blue.
    float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, color);

	// Clear the depth buffer.// Lesson 3D 2
	devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 3D rendering on the back buffer: Start
    
	// Tell the Input-Assembler what vertex buffer to display.
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset); // Bind an array of vertex buffers to the input-assembler stage.
	
	// Tell the Input-Assembler what index buffer to use when drawing. // Lesson 3D 3
	devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);   // Bind an index buffer to the input-assembler stage.

	// Tell the Input-Assembler which primitive type we are using.
    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST was finally changed to D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST in "Lesson 2: Rendering Depth" (Lesson 3D 2)! // Lesson 3D 2

	// Unlike in the prior lesson, only one object is drawn. // Lesson 3D 3
	// 1) UpdateSubresource: Set the new values for the constant buffer: Copy matFinal to the constant buffer pointed to by pCBuffer.
	//    In many cases, you are going to want to update the information in the constant buffer every frame. // Lesson 4
	//    To do this efficiently, Direct3D has a function called UpdateSubresource().
	// 2) DrawIndexed: Instead of using Draw we use DrawIndexed. In this way, Direct3D knows whether to use the index buffer or just render normally. // Lesson 3D 3
	devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal, 0, 0);
	devcon->DrawIndexed(18, 0, 0); // The index buffer object's size is the size of a DWORD struct * 18 // Lesson 3D 3 Exercise 1

	// HERE
	// Exercise 2: Draw 19 more (20 total) copies of the pyramid on the screen at once (in different positions). // Lesson 3D 3 Exercise 2
	float shift = -0.90f; // The initial value of 'shift' sets the position of the 2nd pyramid relative to the first.
	// Random number generator (move it outside RenderFrame()) 
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,4);
	int dice_roll;
	for (int i = 0; i < 19; i++)
	{
		// Create a translation matrix to shift each pyramid to a new position on the screen.
		matTranslate = XMMatrixTranslation(shift, 0.0f, 0.0f); // How can we better distribute the pyramids throughout the screen?
		// Create a new matFinal matrix.
		matFinal = matFinal * matTranslate;  // matFinal's new value is a translation of its prior value.
		// Draw the next object.
		devcon->UpdateSubresource(pCBuffer, 0, 0, &matFinal, 0, 0);
		devcon->DrawIndexed(18, 0, 0);
		// Calculate the new shift.
		dice_roll = distribution(generator); // A random number between 1 and 5. *** The same pattern is drawn with every execution of this program! Changing the value of distribution() gives a new fixed set *** See http://www.cplusplus.com/reference/random/?kw=random for how to change this (search 'seeds').
		shift = (float) dice_roll / 30;      // The updated value of 'shift' sets the position of the 3nd through 20th pyramids relative to the 2nd through 19th pyramids.
	}

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
	
	// 3D rendering on the back buffer: End
}

// This function cleans up Direct3D and COM.
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode (needed when going Fullscreen). I'm leaving this, even when this program is already in windowed mode.

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
	zbuffer->Release(); // Lesson 3D 2
    pVBuffer->Release();
    pCBuffer->Release(); // Lesson 4
	pIBuffer->Release(); // Lesson 3D 3
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}