#pragma once
#include <stb_image.h>
#include <glad/glad.h>
#include <stdint.h>

class Texture
{
	const char* m_fileName{ nullptr };
	int m_width{ 0 };
	int m_height{ 0 };
	int m_numColChannels{ 0 };
	GLuint m_texture;
	char* m_imageData{ nullptr };
	char* Image() { return m_imageData; }
public:
	Texture(const char* filename, unsigned width = 0, unsigned height = 0, unsigned colorChannels = 0);
	const char* FileName() const { return m_fileName; }
	int Width() const { return m_width; }
	int Height() const { return m_height; }
	int Channels() const { return m_numColChannels; }
	const char* Image() const { return m_imageData; }
	int Load();
	void Generate();
	void Delete();
	void SetActive(GLenum texNumber);
	void Bind();
	void Unbind();
	void SetInterpolation(GLenum interpolation);
	void SetRepetitionX(GLenum repition);
	void SetRepetitionY(GLenum repition);
	void Unload();
	void GenerateMipMap() const;
};

