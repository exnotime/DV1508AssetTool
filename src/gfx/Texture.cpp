#include "Texture.h"
#include <Soil2/SOIL2.h>
using namespace gfx;

Texture::Texture()
{
	m_Loaded = false;
	m_Width = 0;
	m_Height = 0;
	m_Channels = 0;
	m_Filename = "";
	m_Handle = 0;
}

Texture::~Texture()
{
	//check up on how to release textures later
	glDeleteTextures(1, &m_Handle);
}

bool Texture::Init(const char* Filename, TextureType type)
{
	m_Filename = std::string(Filename);
	m_Type = type;
	
	if (type == TEXTURE_COLOR || type == TEXTURE_GREYSCALE){
		m_Handle = SOIL_load_OGL_texture(Filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_Width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else if (type == TEXTURE_CUBE){
		m_Handle = SOIL_load_OGL_single_cubemap(Filename, SOIL_DDS_CUBEMAP_FACE_ORDER, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_COMPRESS_TO_DXT);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	printf("Loaded Texture %s \n", Filename);
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
	if (m_Type == TEXTURE_COLOR || m_Type == TEXTURE_GREYSCALE)
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	else
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
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
