#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"
#include "ClickDot.h"

class UVTranslation
{
public:
	//UVTranslation();
	//~UVTranslation();

	void Initialize(int winWidth, int winHeight);
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);
	void Shutdown();

	void AddVertex(int vertex);
	void RemoveVertex(int vertex);

	std::vector<glm::vec2> GetLinePositions();
	
private:
	bool ListContains(int vertex);

	glm::vec3 m_selectedFace;

	int m_winWidth;
	int m_winHeight;

	std::vector<ClickDot> m_dots;

	bool m_uvModeOn;
};

