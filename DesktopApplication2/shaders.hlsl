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
//   A semantic specified after a shader function's parameter list is used to convey information about the intended use of the shader function's return value, e.g., 
//     In "float4 VShader(float4 position3D : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD) : SV_POSITION" the semantic SV_POSITION applies to the simple float4 return value.
//   For a shader function to return multiple variables, it returns a struct containing multiple members, just as a C++ program would. Each structure member must specify its associated semantic.
//     In such a case the shader function no longer specifies its own semantic, i.e., SV_POSITION is removed, and the simple return type is replaced by the struct name, e.g., 
//     "float4 VShader(float4 position3D : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD) : SV_POSITION"
//      becomes:
//     "VOut   VShader(float4 position3D : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)"
// Semantics used in the shader functions defined here are described below.
//
// The vertex shader function executes once for each vertex in the scene.         For a single triangle, it is called by the graphics driver three times, once per vertex.
// The pixel  shader function executes once for each pixel  in the render target. Based on the output clip positions, the rasterizer identifies the pixels covered by each triangle, perspectively interpolates the vertex attributes at those pixels, and then calls the pixel shader for each pixel.
//
// Authorship
// This program, except for the objReader function that parses the Wavefront.obj file, is based on "DirectX 11 Win32 Desktop: Direct3D: Moving to 3D: Lesson 3: Simple Modeling" and earlier lessons by Chris Hanson (https://DirectXTutorial.com).
// All defects in this program are solely the responsibility of Robert John Tortorelli.

// Declarations: Start

// All variables and functions coded in HLSL (.hlsl files) are processed in GPU memory.
// All variables and functions coded in C++  (.cpp  files) are processed in CPU memory.

// Vector variables store between one and four scalar values and are declared by placing a number at the end of the type specification, 
// e.g., a vector variable named "color"  containing four float scaler values can be declared and defined using 
// float4 color = {1.0f, 2.0f, 3.0f, 4.0f};
// OR
// float4 color = float4(1.0f, 2.0f, 3.0f, 4.0f);
//
// Matrix variables store between one and sixteen scaler values and are declared by placing the number of rows and columns at the end of the type specification, 
// e.g., a matrix variable named "matrix" containing four float scaler values can be declared and defined using 
// float2x2 matrix = { 1.0f, 2.0f, // row 1
//                     3.0f, 4.0f  // row 2
//                   };
// OR
// float2x2 matrix = float2x2(1.0f, 2.0f,  // row 1
//                            3.0f, 4.0f); // row 2
//
// HLSL variables may be larger than the corresponding C++ variables used to indirectly (by way of the vertex buffer) assign them data.
// For example, a HLSL float4 variable, such as "position3D" in "float4 position3D : POSITION;" is assigned values from the vertex buffer, that were originally copied to the vertex buffer (using C++), from the smaller C++ variable OurVertices.
// OurVertices only specifies three floats for vertex position, and they are assigned to the first three floats of "position3D", leaving the last float of "position3D" undefined.

// Declare the constant buffer.
// Note this is defined using the type cbuffer, not the type struct.
// See the C++ constant buffer structure declaration for an explanation of these constant buffer members.
cbuffer ConstantBuffer
{
	float4x4 matFinal;
	float4x4 matRotate;
	float4 LightVector;
	float4 LightColor;
	float4 AmbientColor;
}

// Declare the struct of return values output by the vertex shader. It is sometimes also used as the input struct for the pixel shader.
//
// For the vertex shader function to return multiple variables, it returns a struct containing multiple members, just as a C++ program would.
// The order of these struct members determines the order of the input parameters passed to the pixel shader (see the definition of the pixel shader function).
//
// Semantics:
// COLOR:       Pixel color.                                                                                    -> Vertex shader | Vertex shader -> Pixel shader
// TEXCOORD:	Texture Coordinates.																			-> Vertex shader | Vertex shader -> Pixel shader
// SV_POSITION: Vertex position in screen space (2D space: Between 1 and -1 on the X and Y axes).                                  Vertex shader -> Pixel shader
struct VOut
{
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float4 position2D : SV_POSITION;
};

// Declare the texture 2D object.
Texture2D Texture;
// Declare the Sampler Type as SamplerState.
SamplerState ss;

// Declarations: End

// VShader function: Definition
// This function is the vertex shader function.
// A vertex shader function is executed once for each vertex.
// A vertex shader function always operates on a single input vertex and produces a single output vertex.
// A vertex shader function may return multiple variables. In such case it returns a struct, e.g., structure VOut with its member variables, and consequently it specifies the associated semantics in that struct's definition.
// A vertex shader function must return vertex 2D position. Vertex color, vertex lighting, vertex texture coordinates, fog factors, and other characteristics of a single vertex, are optional.
//
// The number of times a vertex shader has been executed can be queried from the CPU using the VSInvocations pipeline statistic.
// 
// The vertex shader's input parameters may be specified in any order.
// The C++ input element description array of structures is used to define the input-layout object that describes the input elements of the C++ VERTEX structure definition. These input element descriptions agree in number and type with the vertex shader's input parameters.
// Because each C++ input element description specifies the HLSL semantic name associated with each input element of the C++ VERTEX structure, the vertex shader's input parameters, which include the HLSL semantic name, may be specified in any order.
//
// Semantics:
// POSITION:    Vertex position in 3D space.                                                                    -> Vertex shader
// NORMAL:      Normal vector.                                                                                  -> Vertex shader
// TEXCOORD:	Texture Coordinates.																			-> Vertex shader | Vertex shader -> Pixel shader
VOut VShader(float4 position3D : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	VOut output;

	output.position2D = mul(matFinal, position3D);          // output.position2D with semantic SV_POSITION = f(constant buffer's matFinal, VShader parameter position3D with semantic POSITION)                       (Position of each vertex in 2D space is a function of the constant buffer's final matrix and the position of the pixel in 3D space)
	output.color = AmbientColor;                            // output.color    with semantic COLOR       = f(constant buffer's ambient light's color)                                                                 (Color of each vertex is set to the ambient light's color, a single color)

	// Calculate changes in color based on the level of light: Calculate the diffuse brightness and combine it with the ambient light's color.
	float4 normalVector = normalize(mul(matRotate, normal));// Rotate the vertex normal vectors to match the rotation of the object. The normalize intrinsic scales the calculated value of a vector to make its length equal to 1.0. Variable norm is necessary because VShader parameter normal cannot be modified.
	float diffusebrightness = saturate(dot(normalVector, LightVector)); // Calculate diffuse brightness as the dot product of the vertex normal vectors and the light vectors. The saturate intrinsic clamps the calculated values between 0 and 1.
	output.color += LightColor * diffusebrightness;         // output.color    with semantic COLOR       = f(constant buffer's ambient light's color, constant buffer's light's color, calculated diffuse brightness) (Color of each vertex is set to the result of this new calculation)
	output.texcoord = texcoord;								// Set the texture coordinates, unmodified.

	return output;
}

// PShader function: Definition
// This function is the pixel shader function.
// A pixel shader function is also known as a fragment shader function.
// A pixel shader function is executed once for each pixel (fragment).
// A pixel shader function's inputs are interpolated from the vertex attributes of the primitive being rasterized, based on the interpolation mode declared.
// A pixel shader function returns a single variable, color. Therefore it does not return a struct, and consequently it specifies the associated semantic in the pixel shader function's definition, i.e., SV_TARGET.
//
// The number of times a pixel shader function has been executed can be queried from the CPU using the PSInvocations pipeline statistic.
//
// When passing multiple variables between shader functions (e.g., return values output by the vertex shader -> input parameters of the pixel shader), they must be passed in the same order, e.g., COLOR first; TEXCOORD second; SV_POSITION third as specified by the order of members in the structure VOut returned by the vertex shader.
// The input parameters of the pixel shader must be a continuous subset of the return values output by the vertex shader, where this subset starts with the first return value of the vertex shader. SV_POSITION, which is not needed by the pixel shader, is therefore placed as the third member of the structure VOut so it need not be passed as input to the pixel shader.
// It is possible for the pixel shader to have fewer input parameters than the return values of the vertex shader, as the pixel shader only needs to know the interpolated data for each pixel and not the full set of data for the entire model.
//   For example:
//   If Vertex shader output is: a, b, c then:
//   Pixel shader input: a, b, c (valid)
//   Pixel shader input: a, b    (valid)
//   Pixel shader input: a       (valid)
//   Pixel shader input: b, c    (invalid)
//
// Semantics:
// COLOR:       Pixel color.                                                                                    -> Vertex shader | Vertex shader -> Pixel shader
// TEXCOORD:	Texture Coordinates.																			-> Vertex shader | Vertex shader -> Pixel shader
// SV_TARGET:   Final color of the pixel of the render target.                                    Pixel shader  -> (Output-Merger Stage)
float4 PShader(float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * Texture.Sample(ss, texcoord);
}