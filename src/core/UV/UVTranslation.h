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

	//void SelectFace(int vertex1, int vertex2, int vertex3);
	void AddVertex(int vertex);
	void RemoveVertex(int vertex);
	
private:
	bool ListContains(int vertex);

	glm::vec3 m_selectedFace;

	int m_winWidth;
	int m_winHeight;

	//gfx::Sprite m_dotSprite1;
	//gfx::Sprite m_dotSprite2;
	//gfx::Sprite m_dotSprite3;
	//float* m_uvPos1;
	//float* m_uvPos2;
	//float* m_uvPos3;

	//gfx::Sprite m_dotSprite4;
	//float* m_uvPos4;

	std::vector<ClickDot> m_dots;

	bool m_uvModeOn;
};

