#version 430 core
out vec4 FragColor;

//get the vec3 color of vertices from Vertex Shader
in vec3 color;

//get the vec2 texture coordinates from Vertex Shader
in vec2 texCoordinates;

uniform sampler2D tex0;

in float fs_pulsateColor;

layout (std430, binding=2) buffer shader_data
{ 
  vec4 posArray;
};

void main()
{
	//FragColor = vec4(color * fs_pulsateColor, 1.0f);
	//FragColor = texture(tex0,vec2(texCoordinates.x * fs_pulsateColor,texCoordinates.y * fs_pulsateColor));
	FragColor = texture(tex0,vec2(texCoordinates.x * posArray.x,texCoordinates.y * posArray.y));
}