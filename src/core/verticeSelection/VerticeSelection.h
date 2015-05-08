#pragma once

#include <vector>
#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

class VerticeSelection{
public:
	VerticeSelection();
	~VerticeSelection();

	void					Initialize();
	bool					Update(const float deltaTime);
	void					Draw(gfx::RenderQueue* renderQueue, gfx::RenderObject ro );

private:
	std::vector<unsigned int>	m_SelectedVertices;
	bool m_Checked;
};