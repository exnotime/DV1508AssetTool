#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <string>

typedef int TextureHandle;
namespace gfx
{
	enum TextureType
	{
		TEXTURE_2D,
		CUBE_TEXTURE
	};

	class Texture
	{
	public:
		Texture();
		~Texture();

	 bool Init( const char* Filename);
	 GLuint GetHandle();
	 void Apply( GLuint location, int index );
	 bool GetLoaded();
	 void SetFilename( const char* filename );
	 std::string GetFilename();

	private:
		GLuint m_Handle;
		int m_Width, m_Height, m_Channels;
		bool m_Loaded;
		std::string m_Filename;
	};
}
