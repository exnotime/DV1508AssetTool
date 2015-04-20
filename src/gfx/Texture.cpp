#include "Texture.h"
#include "stb_image/stb_image.h"
using namespace gfx;

Texture::Texture()
{
	m_Loaded = false;
	m_Width = 0;
	m_Height = 0;
	m_Filename = "";
	m_Handle = 0;
}

Texture::~Texture()
{
	//check up on how to release textures later
	glDeleteTextures(1, &m_Handle);
}

bool Texture::Init(const char* Filename)
{
	m_Filename = std::string(Filename);

	unsigned char* data;
	FILE* file = fopen(Filename, "rb");
	if (!file){
		printf("Error loading texture: %s\n", Filename);
		return false;
	}
	data = stbi_load_from_file(file, &m_Width, &m_Height, &m_Channels, 0);

	glGenTextures(1, &m_Handle);
	glBindTexture(GL_TEXTURE_2D, m_Handle);
	GLenum format, internalFormat;
	if (m_Channels == 3){
		format = GL_RGB;
		internalFormat = GL_RGB8;
	}
	else {
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
	}
		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	printf("Loaded Texture %s \n", Filename);
	fclose(file);
	return true;
}

GLuint Texture::GetHandle()
{
	return m_Handle;
}

void Texture::Apply(GLuint location,int index)
{
	glUniform1i(location,index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_Handle);
}

bool Texture::GetLoaded(){
	return m_Loaded;
}

void Texture::SetFilename(const char* filename){
	m_Filename = std::string(filename);
}

std::string Texture::GetFilename(){
	return m_Filename;
}
