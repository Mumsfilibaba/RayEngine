#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
	vec4 gl_Position;
};

vec2 positions[3] = vec2[](
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
	);

void main() {
	gl_Position = vec4(positions[0], 0.0, 1.0);
}

/*struct VS_IN
//{
//	float3 Position : POSITION0;
//};
//
//struct VS_OUT
//{
//	float4 Position : SV_POSITION;
//};
//
//VS_OUT main(VS_IN input)
//{
//	VS_OUT output;
//	output.Position = float4(input.Position, 1.0f);
//
//	return output;
//}*/