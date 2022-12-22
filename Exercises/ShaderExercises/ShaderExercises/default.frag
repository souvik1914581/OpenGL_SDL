#version 330 core
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;

void main()
{
	vec3 invertedColor = vec3(1.0f,1.0f,1.0f) - color;
	FragColor = vec4(invertedColor, 1.0f);
}