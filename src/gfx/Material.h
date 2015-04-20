#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::ivec4;
namespace gfx
{
    class Material
    {
    public:
        Material(){ };
		~Material(){
			if (m_AlbedoTexture)	delete m_AlbedoTexture;
			if (m_NormalTexture)	delete m_NormalTexture;
			if (m_RoughnessTexture) delete m_RoughnessTexture;
			if (m_MetalTexture)		delete m_MetalTexture;
		};
        //Getters
        std::string& GetName(){ return m_Name; }

        Texture* GetAlbedoTexture()						{ return m_AlbedoTexture; }
        Texture* GetNormalTexture()						{ return m_NormalTexture; }
        Texture* GetRoughnessTexture()					{ return m_RoughnessTexture; }
        Texture* GetMetalTexture()						{ return m_MetalTexture; }
        //Setters
        void SetName(std::string name)						{ m_Name = name; }
        void SetAlbedoTexture	( Texture* tex)		{ m_AlbedoTexture = tex; }
        void SetNormalTexture	( Texture* tex)		{ m_NormalTexture = tex; }
        void SetRoughnessTexture( Texture* tex)		{ m_RoughnessTexture = tex; }
		void SetMetalTexture	( Texture* tex)		{ m_MetalTexture = tex; }
    private:
		Texture* m_AlbedoTexture	= nullptr;
		Texture* m_NormalTexture	= nullptr;
		Texture* m_RoughnessTexture = nullptr;
		Texture* m_MetalTexture		= nullptr;
        std::string m_Name = "";
    };
}
