#pragma once
#include <glm/glm.hpp>
#include "MaterialBank.h"
namespace gfx{
	class Sprite{
	public:
		Sprite();
		~Sprite();
		Sprite(const char* filename);
		void SetPos(const glm::vec2& pos);//in pixels
		void SetSize(const glm::vec2& size); //in pixels
		void SetColor(const glm::vec4 color);
		void SetTexture(const char* filename);
		void SetTexture(TextureHandle tex);
		glm::vec2& GetPos();//in pixels
		glm::vec2& GetSize();//in pixels
		glm::vec4& GetPosFlt();//used by the gfx engine
		glm::vec4& GetSizeFlt();//used by the gfx engine
		glm::vec4& GetColor();
		TextureHandle GetTexture();
	private:
		glm::vec4		m_Pos;
		glm::vec4		m_Size;
		glm::vec4		m_Color;
		TextureHandle	m_Texture;
	};
}
