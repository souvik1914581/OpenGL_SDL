#version 330 core
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}