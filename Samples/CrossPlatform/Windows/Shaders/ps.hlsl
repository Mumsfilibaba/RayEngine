struct PS_IN
{
	float4 Position : SV_POSITION;
};

cbuffer ColorBuffer : register(b0)
{
	float4 g_Color;
};

float4 main(PS_IN input) : SV_TARGET
{
	return g_Color;
}