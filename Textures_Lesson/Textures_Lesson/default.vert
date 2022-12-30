 #version 330 core

 //layout 0 is used for x,y coordinates
layout (location = 0) in vec3 aPos;

//layout 1 is used for R,G,B values
layout (location = 1) in vec3 aColor;

//layout 2 is used for texture coordinates
layout(location = 2) in vec2 aTex;

//color is handled by Fragment Shader, so Vertex Shader needs to out it
out vec3 color;

//color is handled by Fragment Shader, so Vertex Shader needs to out Texture coordinates
out vec2 texCoordinates;

out float fs_pulsateColor;

//uniforms are like universal variables which can be used by any Shader and accessed from main() without VAO
uniform float scale;
//pulsate colors
uniform float pulsateFactor;
void main()
{
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	color = aColor;
	fs_pulsateColor = pulsateFactor;
	texCoordinates = aTex;
}