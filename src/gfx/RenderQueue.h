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
	struct GizmoObject{
		glm::mat4x4 world;
		ModelHandle	Model;
		glm::vec4	Color;
	};
	struct LineObject{
		std::vector<glm::vec2> Lines;
		glm::vec4 Color;
		//float scale;
	};

	class RenderQueue{
	public:
		RenderQueue( );
		~RenderQueue( );
		void Enqueue( const RenderObject& ro );
		void Enqueue( const Sprite& spr, int layer = 0 );
		void Enqueue( const BrushObject& bo );
		void Enqueue( const GizmoObject& go );
		void Enqueue( const LineObject& lo);
		std::vector<RenderObject>& GetQueue( );
		std::vector<std::vector<Sprite>>& GetSpriteQueue();
		std::vector<BrushObject>& GetBrushQueue();
		std::vector<GizmoObject>& GetGizmoQueue();
		std::vector<LineObject>& GetLineQueue();
		void SetTargetTexture(TextureHandle handle);
		TextureHandle GetTargetTexture();
		void Clear();
	private:
		std::vector<RenderObject> m_Queue;
		std::vector<std::vector<Sprite>> m_SpriteQueue;
		TextureHandle m_TargetTexture;
		std::vector<BrushObject> m_BrushQueue;
		std::vector<GizmoObject> m_GizmoQueue;
		std::vector<LineObject> m_LineQueue;
	};
};
