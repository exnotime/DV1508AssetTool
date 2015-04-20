#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "ShaderDefineContainer.h"
#include "Texture.h"
#include "ShaderProgramBinary.h"
#include <vector>
#include <map>
using std::ifstream;

namespace gfx
{
class ShaderProgram
{
public:
	 ShaderProgram ( void );
	 ShaderProgram ( std::vector<Shader*> Shaders );
	 ~ShaderProgram ( void );
	 bool Init ( std::vector<Shader*> Shaders, bool print );
	 GLuint GetHandle();
	 std::string& GetFilename();
	 void Apply();
	 bool Validate();
	 GLuint GetUniformLocationByName ( const std::string& name );
	 bool LoadCompleteShaderProgramFromFile ( const std::string& filename, bool print );
	 bool LoadCompleteShaderProgramFromFile ( const std::string& filename, bool print, ShaderDefineContainer& container );
	 void SetUniformFloat ( const std::string& name, const float value );
	 void SetUniformInt ( const std::string& name, const int value );
	 void SetUniformUInt ( const std::string& name, const unsigned int value );
	 void SetUniformBool ( const std::string& name, const bool value );
	 void SetUniformVec3 ( const std::string& name, const glm::vec3& value );
	 void SetUniformMat4 ( const std::string& name, const glm::mat4x4& value );
	 void SetUniformVec2 ( const std::string& name, const glm::vec2& value );
	 void SetUniformVec4 ( const std::string& name, const glm::vec4& value );
	 void SetUniformTexture(const std::string& name, Texture& tex);
	 void SetUniformTextureHandle(const std::string& name, GLuint tex, int index);
	 GLint FetchUniform ( const std::string& name );
	 bool Recompile();

	void SaveProgramBinary();
	void LoadProgramBinary(const std::string& filename);
	bool LoadProgramBinaryHeader(const std::string& filename, ShaderProgramBinary& outHeader);
private:
    GLuint                  m_Handle;
    std::vector<Shader*>		m_Shaders;
    bool                    m_LoadedShaders;
    std::map<std::string, GLint>    m_UniformMap;
    int                     m_TextureCount;
    bool                    m_Validated = false;
    std::string                 m_Filename;
    ShaderDefineContainer   m_Defines;

    void CreateShader ( ifstream* FileStream, GLenum shaderType, const std::string& filename, bool print );
    void CreateShader ( ifstream* FileStream, GLenum shaderType, const std::string& filename, bool print, ShaderDefineContainer& container );
    
    std::string LoadText ( const char* filename );
};
}

