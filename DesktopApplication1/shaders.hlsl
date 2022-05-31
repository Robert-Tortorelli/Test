// DirectX 11 Win32 Desktop  Direct3D: Moving to 3D: Lesson 1: 3D Transformations
cbuffer ConstantBuffer
{
	// The names of the corresponding C++ struct and the variables in this shader do not have to match, 
	// but the order of the variables must match.
	float4x4 matFinal; // A float4x4 is 16 (4x4) floats (each 4 bytes) for a total of 64 bytes.
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = mul(matFinal, position);
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}
