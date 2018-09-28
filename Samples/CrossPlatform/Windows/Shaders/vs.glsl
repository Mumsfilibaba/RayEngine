#version 330

layout(location = 0) in vec3 gPosition; 

void main()
{
    gl_Position = vec4(gPosition, 1.0f);
}
