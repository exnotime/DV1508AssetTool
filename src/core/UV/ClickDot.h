#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"


class ClickDot
{
public:
	ClickDot();
	~ClickDot();

	void Initialize(int index, const char* filename);
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);

	float U();
	float V();
	int Index();

	float X();
	float Y();
private:
	gfx::Sprite m_dot;
	float m_radius;

	//float m_winHeight;
	//float m_winWidth;
	//float m_winHWidth;
	//float m_winHHeight;

	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
	float m_width;
	float m_height;

	bool m_clickedLastFrame;
	bool m_clickedThisFrame;
	bool m_moveableDot;

	int m_index;
};