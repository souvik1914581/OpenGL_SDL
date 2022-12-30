#version 330 core
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;

//get the vec2 texture coordinates from Vertex Shader
in vec2 texCoordinates;

uniform sampler2D tex0;

in float fs_pulsateColor;
void main()
{
	//FragColor = vec4(color * fs_pulsateColor, 1.0f);
	FragColor = texture(tex0,texCoordinates);
}