struct VS_IN
{
	float3 Position : POSITION0;
};

struct VS_OUT
{
	float4 Position : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.Position = float4(input.Position, 1.0f);

	return output;
}