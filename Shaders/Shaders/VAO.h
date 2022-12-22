#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	// Links a VBO to the VAO using a certain layout
	/*
		Vertex1		|		Vertex2
		____________________________
		X Y Z R G B	|	X Y Z R G B
		^				^
		|_______________|
			Coordinate Stride
		____________________________
		X Y Z R G B	|	X Y Z R G B
			  ^               ^
			  |_______________|
			    Color Stride
		size of each Vertex = 6 * sizeof(GLfloat)
		color offset = 3 * sizeof(GLfloat)
		layout 0:	Coordinates/Position
		layout 1:	RGB values
		numcomponents in each layout = 3
	*/
	void LinkAttrib(VBO& VBO, GLuint layout,GLuint numComponents,GLenum type,GLsizeiptr stride,void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};
#endif