#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"
#include "ClickDot.h"

class UVTranslation
{
public:
	//UVTranslation();
	//~UVTranslation();

	void Initialize();
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);
	void Shutdown();

	void AddVertex(int vertex);
	void RemoveVertex(int vertex);
	void ResetList();
	
	void Enable();
	void Disable();
	void Toggle();
	bool IsActive();
	
private:
	bool ListContains(int vertex);
	std::vector<glm::vec2> GetLinePositions();

	std::vector<ClickDot> m_dots;

	bool m_uvModeOn;
};

