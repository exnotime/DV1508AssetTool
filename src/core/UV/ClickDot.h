#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"


class ClickDot
{
public:
	ClickDot();
	~ClickDot();

	void Initialize(int index, int winWidth, int winHeight, const char* filename);
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);

	float U();
	float V();
	int Index();
private:
	float X();
	float Y();

	gfx::Sprite m_dot;
	float m_radius;

	float m_winHeight;
	float m_winWidth;
	float m_winHWidth;
	float m_winHHeight;

	bool m_clickedLastFrame;
	bool m_clickedThisFrame;
	bool m_moveableDot;

	int m_index;
};