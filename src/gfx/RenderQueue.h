#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ModelBank.h"
#include "Sprite.h"
namespace gfx{
	struct RenderObject{
		glm::mat4x4 world;
		ModelHandle		Model;
	};
	class RenderQueue{
	public:
		RenderQueue( );
		~RenderQueue( );
		void Enqueue( const RenderObject& ro );
		void Enqueue(const Sprite& spr, int layer = 0);
		std::vector<RenderObject>& GetQueue( );
		std::vector<std::vector<Sprite>>& GetSpriteQueue();
		void SetTargetTexture(TextureHandle handle);
		TextureHandle GetTargetTexture();
		void Clear();
	private:
		std::vector<RenderObject> m_Queue;
		std::vector<std::vector<Sprite>> m_SpriteQueue;
		TextureHandle m_TargetTexture;
	};
};
