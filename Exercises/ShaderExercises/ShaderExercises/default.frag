#version 330 core
precision mediump float;
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;
in float invertColor;
void main()
{
	vec3 effectiveColor = color;
	if(invertColor)
	{
		effectiveColor = vec3(1.0f,1.0f,1.0f) - color;
	}
	FragColor = vec4(effectiveColor, 1.0f);
}