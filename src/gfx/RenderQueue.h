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
	struct BrushObject{
		glm::vec2		Position;
		float			Size;
		TextureHandle	Texture;
	};
	class RenderQueue{
	public:
		RenderQueue( );
		~RenderQueue( );
		void Enqueue( const RenderObject& ro );
		void Enqueue(const Sprite& spr, int layer = 0);
		void Enqueue(const BrushObject& bo);
		std::vector<RenderObject>& GetQueue( );
		std::vector<std::vector<Sprite>>& GetSpriteQueue();
		std::vector<BrushObject>& GetBrushQueue();
		void SetTargetTexture(TextureHandle handle);
		TextureHandle GetTargetTexture();
		void Clear();
	private:
		std::vector<RenderObject> m_Queue;
		std::vector<std::vector<Sprite>> m_SpriteQueue;
		TextureHandle m_TargetTexture;
		std::vector<BrushObject> m_BrushQueue;
	};
};
