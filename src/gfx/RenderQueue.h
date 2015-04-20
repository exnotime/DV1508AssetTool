#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ModelBank.h"
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
		std::vector<RenderObject>& GetQueue( );
		void Clear();
	private:
		std::vector<RenderObject> m_Queue;
	};
};
