#include "ClickDot.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "GraphicsEngine.h"

ClickDot::ClickDot(){}
ClickDot::~ClickDot(){}

void ClickDot::Initialize(int index, int winWidth, int winHeight, const char* filename)
{
	m_index = index;
	m_winHeight = (float)winHeight;
	m_winWidth = (float)winWidth;
	m_winHHeight = m_winHeight * 0.5f;
	m_winHWidth = m_winWidth * 0.5f;

	m_dot.SetTexture(filename);
	m_dot.SetPos(glm::vec2(m_winHWidth, 0.0f));

	m_clickedThisFrame = false;
	m_clickedLastFrame = false;
	m_moveableDot = false;

	float width = m_dot.GetSizeFlt().x * gfx::g_GFXEngine.GetWidth();
	float height = m_dot.GetSizeFlt().y * gfx::g_GFXEngine.GetHeight();
	m_radius = (width + height) * 0.5f;
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
			float distance = sqrtf((mPos.x - x) * (mPos.x - x) + (mPos.y - y) * (mPos.y - y));

			// Clicked inside dot
			if (distance < m_radius)
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

int ClickDot::Index()
{
	return m_index;
}