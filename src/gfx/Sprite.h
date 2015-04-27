#pragma once
#include <glm/glm.hpp>
#include "MaterialBank.h"
namespace gfx{
	class Sprite{
	public:
		Sprite();
		~Sprite();
		Sprite(const char* filename);
		void SetPos(const glm::vec2& pos); //in pixels
		void SetSize(const glm::vec2& size); // in pixels
		void SetTexture(const char* filename);
		glm::vec2& GetPos();//in pixels
		glm::vec2& GetSize();//in pixels
		glm::vec4& GetPosFlt();//used by the gfx engine
		glm::vec4& GetSizeFlt();//used by the gfx engine
		TextureHandle GetTexture();
	private:
		glm::vec4		m_Pos;
		glm::vec4		m_Size;
		TextureHandle	m_Texture;
	};
}
