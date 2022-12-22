 #version 330 core

 //layout 0 is used for x,y coordinates
layout (location = 0) in vec3 aPos;

//layout 1 is used for R,G,B values
layout (location = 1) in vec3 aColor;

//color is handled by Fragment Shader, so Vertex Shader needs to out it
out vec3 color;
out float invertColor;

//uniforms are like universal variables which can be used by any Shader and accessed from main() without VAO
uniform float scale;
uniform int flip;
uniform bool invertColorFlag;
void main()
{
	float yPos = aPos.y + aPos.y * scale;
	if(flip == 1)
	{
		yPos = -yPos;
	}
	gl_Position = vec4(aPos.x + aPos.x * scale,yPos, aPos.z + aPos.z * scale, 1.0);
	color = aColor;
	if(invertColorFlag)
	{
		invertColor = 1.0f;
	}
	else
	{
		invertColor = 0.0f;
	}
}