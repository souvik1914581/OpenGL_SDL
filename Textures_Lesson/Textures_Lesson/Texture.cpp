#include "Texture.h"

Texture::Texture(const char* filename,unsigned width, unsigned height, unsigned colorChannels):m_width(width),m_height(height),m_numColChannels(colorChannels),m_fileName(filename)
{
	glGenTextures(1, &m_texture);
}

int Texture::Load()
{
	//stbi reads images from top left to bottom right, which is opposite to OpenGL
	stbi_set_flip_vertically_on_load(true);

	m_imageData = (char*) (stbi_load(FileName(), &m_width, &m_height, &m_numColChannels, 0));
	int retVal = (nullptr == m_imageData) ? -1 : 0;
	return retVal;
}

void Texture::Generate()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width(), Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image());
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::SetActive(GLenum texNum)
{
	glActiveTexture(texNum);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetInterpolation(GLenum interpolation)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
}

void Texture::SetRepetitionX(GLenum repetition)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repetition);
}

void Texture::SetRepetitionY(GLenum repetition)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repetition);
}

void Texture::Unload()
{
	stbi_image_free(Image());
}

void Texture::GenerateMipMap() const
{
	glGenerateMipmap(GL_TEXTURE_2D);
}
