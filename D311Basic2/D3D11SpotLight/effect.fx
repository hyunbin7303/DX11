struct Light
{
	float3 pos;
	float  range;
	float3 dir;
	float cone;
	float3 att;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;
	output.Pos = mul(inPos, WVP);
	output.worldPos = mul(inPos, World);
	output.normal = mul(normal, World);
	output.TexCoord = inTexCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal); // x/ length(x)
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightToPixelVec = light.pos - input.worldPos;
	float d = length(lightToPixelVec); // distance between light position and pixel position.
	float3 finalAmbient = diffuse * light.ambient; // Add the ambient light.

	// If pixel is too far, return pixel color with ambient light.
	if (d > light.range)
	{
		return float4(finalAmbient, diffuse.a);
	}

	// Turn lightToPixelVec into a unit length vector describing the pixels direction from the light position.
	lightToPixelVec /= d;

	float howMuchLight = dot(lightToPixelVec, input.normal);
	if (howMuchLight > 0.0f)
	{
		finalColor += diffuse * light.diffuse;
		finalColor /= (light.att[0] + (light.att[1] * d)) + (light.att[2] * (d*d));

		// Calculate falloff from center to edge of pointlight cone
		finalColor *= pow(max(dot(-lightToPixelVec, light.dir), 0.0f), light.cone);
	}
	finalColor = saturate(finalColor + finalAmbient); 
	return float4(finalColor, diffuse.a);
}
	
float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	return diffuse;
}