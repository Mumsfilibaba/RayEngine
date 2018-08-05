#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(1.0, 0.0, 0.0, 1.0);
}

/*struct PS_IN
{
	float4 Position : SV_POSITION;
};

float4 main(PS_IN input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}*/