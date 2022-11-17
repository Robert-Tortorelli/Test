// Shader Functions
// Version 3.1
//
// Description
// Shader functions are small, low-level, programs that are compiled by the CPU and then run by the GPU at specific stages in the graphics pipeline. They automatically receive data from shader functions in previous stages, and return data to shader functions in successive stages, of the graphics pipeline.
// Shader functions execute when the ID3D11DeviceContext::DrawIndexed member function, or a similar member function, is called from a DirectX 11 Direct3D Win32 desktop application.
// The shader functions defined here provide the following functionality:
//   The vertex shader function uses a constant buffer to change the position of each vertex in the render target to a new position, while also mapping each vertex from 3D space to 2D space, i.e., SV_POSITION = f(constant buffer, POSITION)
//   The pixel shader function does nothing.
//   These shader functions leave the color of each pixel unchanged.
//
// A semantic is an all caps string attached to a shader function's input or output that conveys information about the intended use of a parameter on the GPU. Semantics are required on all variables passed between shader stages.
//   A semantic specified after a shader function's parameter list is used to convey information about the intended use of the shader function's return value, e.g., In 'float4 PShader(VOut input) : SV_TARGET' the SV_TARGET semantic applies to the float4 return value.
//   For a shader function to return multiple variables, it returns a struct containing multiple members, just as a C program would. Each structure member must specify a semantic.
//     In such a case the shader function does not specify its own semantic, i.e., SV_POSITION is removed (and the type float4 is replaced by the struct name) from 'float4 VShader(float4 position : POSITION, float4 color : COLOR) : SV_POSITION'
// Semantics used in the shader functions defined here are described below. The relationship of these semantics is as follows:
//   POSITION of each pixel in 3D space & COLOR of each pixel -> Vertex shader -> SV_POSITION of each pixel in 2D space & COLOR of each pixel -> Pixel shader -> SV_TARGET (final color) of each pixel of the render target
//
// The vertex shader function executes once for each vertex in the scene. For a single triangle, it is called by the graphics driver three times, once per vertex.
// The pixel shader function executes once for each pixel in the render target. Based on the output clip positions, the rasterizer identifies the pixels covered by each triangle, perspectively interpolates the vertex attributes at those pixels, and then calls the pixel shader for each pixel.
//
// Authorship
// This program, except for the objReader function that parses the Wavefront.obj file, is based on "DirectX 11 Win32 Desktop: Direct3D: Moving to 3D: Lesson 3: Simple Modeling" and earlier lessons by Chris Hanson (https://DirectXTutorial.com).
// All defects in this program are solely the responsibility of Robert John Tortorelli.

// Declarations: Start

// All variables and functions coded in HLSL (.hlsl files) are stored in GPU memory.
// All variables and functions coded in C++ (.cpp files) are stored in CPU memory.

// Vector variables store between one and four    scalar values and are declared by placing a   number                     at the end of the type, 
// e.g., a vector variable named "color"  containing four float scaler values can be declared and defined using 
// float4 color = {1.0f, 2.0f, 3.0f, 4.0f};
// OR
// float4 color = float4(1.0f, 2.0f, 3.0f, 4.0f);
//
// Matrix variables store between one and sixteen scaler values and are declared by placing the number of rows and columns at the end of the type, 
// e.g., a matrix variable named "matrix" containing four float scaler values can be declared and defined using 
// float2x2 matrix = { 1.0f, 2.0f, // row 1
//                     3.0f, 4.0f  // row 2
//                   };
// OR
// float2x2 matrix = float2x2(1.0f, 2.0f,  // row 1
//                            3.0f, 4.0f); // row 2
//
// HLSL variables may be larger than the corresponding C++ variables used to indirectly (by way of the vertex buffer) assign them data.
// For example, a HLSL float4 variable, such as "position" in "float4 position : SV_POSITION;", is assigned values from the vertex buffer that were copied to the vertex buffer from the smaller C++ variable OurVertices. OurVertices only specifies three floats for vertex position and they are assigned to the first three floats of "position", leaving the last float of "position" undefined.

// Declare the constant buffer structure.
cbuffer ConstantBuffer
{
	float4x4 matFinal;                                      // The final transform matrix.
    float4x4 matRotateY;                                    // The rotation matrix.
    float4 lightvec;                                        // The light's vector.
    float4 lightcol;                                        // The light's color.
    float4 ambientcol;                                      // The ambient light's color.
}

// Declare the Vertex shader output struct. It is also used as the input struct for the Pixel shader.
// The program that is using the shaders defined below specifies a vertex buffer containing multiple values. Therefore multiple variables are returned by the vertex shader function.
//
// For the vertex shader function to return these multiple variables, it returns a struct containing multiple members, just as a C program would.
//
// Semantics:
// SV_POSITION: Vertex position in screen space (2D space: Between 1 and -1 on the X and Y axes).                                  Vertex shader -> Pixel shader
// COLOR:       Pixel color.                                                                                    -> Vertex shader | Vertex shader -> Pixel shader
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// Declarations: End

// VShader function: Definition
// This function is the vertex shader function.
// A vertex shader function is executed once for each vertex.
// A vertex shader function always operates on a single input vertex and produces a single output vertex.
//   This particular vertex shader function returns multiple variables, position and color. Therefore it does return a struct, and consequently it specifies the associated semantics in that struct's definition, i.e., SV_POSITION and COLOR.
// Optionally, this shader can output vertex texture coordinates, vertex color, vertex lighting, fog factors, and other characteristics of a single vertex.
//
// The number of times a vertex shader has been executed can be queried from the CPU using the VSInvocations pipeline statistic.
//
// When passing multiple variables between shader functions, they must be passed in the same order, e.g., position first; color second.
//
// Semantics:
// POSITION:    Vertex position in 3D space.                                                                    -> Vertex shader
// NORMAL:      Normal vector.                                                                                  -> Vertex shader
// SV_POSITION: Vertex position in screen space (2D space: Between 1 and -1 on the X and Y axes).                                  Vertex shader -> Pixel shader
VOut VShader(float4 position : POSITION, float4 normal : NORMAL)
{
    VOut output;

    output.position = mul(matFinal, position);              // output.position with semantic SV_POSITION = f(constant buffer's matFinal, VShader parameter position with semantic POSITION)                           (Position of each vertex in 2D space is a function of the constant buffer's final matrix and the position of the pixel in 3D space)
    output.color = ambientcol;                              // output.color    with semantic COLOR       = f(constant buffer's ambient light's color)                                                                 (Color of each vertex is set to the ambient light's color, a single color)

    // Calculate changes in color based on the level of light: Calculate the diffuse light color and add it to the ambient light's color.
    float4 norm = normalize(mul(matRotateY, normal));       // Rotate the vertex normal to match the rotation of the object. The normalize intrinsic scales the calculated value of a vector to make its length equal to 1.0. Variable norm is necessary because VShader parameter normal cannot be modified.
    float diffusebrightness = saturate(dot(norm, lightvec));// Calculate the diffuse brightness as the dot product of the vertex normal and the light's vector. The saturate intrinsic clamps the calculated value between 0 and 1.
    output.color += lightcol * diffusebrightness;           // output.color    with semantic COLOR       = f(constant buffer's ambient light's color, constant buffer's light's color, calculated diffuse brightness) (Color of each vertex is set to the result of this new calculation)

    return output;
}

// PShader function: Definition
// This function is the pixel shader function.
// A pixel shader function is also known as a fragment shader function.
// A pixel shader function is executed once for each pixel (fragment).
// A pixel shader function's inputs are interpolated from the vertex attributes of the primitive being rasterized, based on the interpolation mode declared.
// A pixel shader function's output is color.
//   This particular pixel shader function returns a single variable, color. Therefore it does not return a struct, and consequently it specifies the associated semantic in the pixel shader function's definition, i.e., SV_TARGET.
//
// The number of times a pixel shader function has been executed can be queried from the CPU using the PSInvocations pipeline statistic.
//
// When passing multiple variables between shader functions, they must be passed in the same order, e.g., position first; color second.
//
// Semantics:
// SV_POSITION: Vertex position in screen space (2D space: Between 1 and -1 on the X and Y axes).                                  Vertex shader -> Pixel shader
// COLOR:       Pixel color.                                                                                    -> Vertex shader | Vertex shader -> Pixel shader
// SV_TARGET:   Final color of the pixel of the render target.                                    Pixel shader  -> (Output-Merger Stage)
//
// The following syntax was provided by DirectXTutorial.com:
// float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
// {
//     return color;                                           // color           is COLOR       = Pixel color (unchanged in this shader function)
// }
// The following alternative syntax is also valid: It was suggested by GitHub Copilot.
// This syntax explicitly shows that the return value of the vertex shader function is being passed to the pixel shader function.
float4 PShader(VOut input) : SV_TARGET
{
    return input.color;                                     // input.color     is COLOR       = Pixel color (unchanged in this shader function)
}