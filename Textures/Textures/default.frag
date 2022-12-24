#version 330 core
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;

in float fs_pulsateColor;
void main()
{
	FragColor = vec4(color * fs_pulsateColor, 1.0f);
}