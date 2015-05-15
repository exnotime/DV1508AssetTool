#include "ClickDot.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "GraphicsEngine.h"

ClickDot::ClickDot(){}
ClickDot::~ClickDot(){}

void ClickDot::Initialize()
{
	m_winHeight = 900.0f;
	m_winWidth = 1600.0f;
	m_winHHeight = m_winHeight * 0.5f;
	m_winHWidth = m_winWidth * 0.5f;

	m_dot.SetTexture("asset/Cube/rDot.png");
	m_dot.SetPos(glm::vec2(m_winHWidth, 0.0f));

	m_clickedThisFrame = false;
	m_clickedLastFrame = false;
	m_moveableDot = false;
}

void ClickDot::Update(const float deltaTime)
{
	m_clickedLastFrame = m_clickedThisFrame;
	ImGuiIO& io = ImGui::GetIO();
	//0 is left button

	if (io.MouseDown[0])
	{
		m_clickedThisFrame = true;
		ImVec2 mPos = io.MousePos;

		// Clicked
		if (!m_clickedLastFrame)
		{
			float x = X();
			float y = Y();
			float width = m_dot.GetSizeFlt().x * gfx::g_GFXEngine.GetWidth();
			float height = m_dot.GetSizeFlt().y * gfx::g_GFXEngine.GetHeight();
			// Clicked inside dot
			if (mPos.x < x + width && mPos.x > x - width &&
				mPos.y < y + height && mPos.y > y - height)
			{
				m_moveableDot = true;
			}
		}
		else
		{
			if (m_moveableDot)
			{
				float mPosX = mPos.x;
				float mPosY = mPos.y;
				if (mPosX < m_winHWidth)
				{
					mPosX = m_winHWidth;
				}
				m_dot.SetPos(glm::vec2(mPosX, mPosY));
			}
		}
	}
	else
	{
		m_clickedThisFrame = false;
	}
}

void ClickDot::Draw(gfx::RenderQueue* renderQueue)
{
	renderQueue->Enqueue(m_dot);
}

float ClickDot::X()
{
	return m_dot.GetPos().x;
}

float ClickDot::Y()
{
	return gfx::g_GFXEngine.GetHeight() - m_dot.GetPos().y;
}

float ClickDot::U()
{
	return (X() - m_winHWidth) / m_winHWidth;
}

float ClickDot::V()
{
	return Y() / m_winHeight;
}