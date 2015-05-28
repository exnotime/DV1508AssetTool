#include "ClickDot.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "GraphicsEngine.h"

ClickDot::ClickDot(){}
ClickDot::~ClickDot(){}

void ClickDot::Initialize(int index, const char* filename)
{
	m_index = index;
	//m_winHeight = (float)winHeight;
	//m_winWidth = (float)winWidth;
	//m_winHeight = gfx::g_GFXEngine.GetHeight();
	//m_winWidth = gfx::g_GFXEngine.GetWidth();
	//m_winHHeight = m_winHeight * 0.5f;
	//m_winHWidth = m_winWidth * 0.5f;

	m_left = gfx::g_GFXEngine.GetWidth() * 0.5f;;
	m_right = gfx::g_GFXEngine.GetWidth();
	m_top = BUTTON_SIZE;
	m_bottom = gfx::g_GFXEngine.GetHeight();;

	m_width = m_right - m_left;
	m_height = m_bottom - m_top;

	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	m_dot.SetTexture(filename);
	m_dot.SetPos(glm::vec2(m_left, m_top));
	float width = m_dot.GetSizeFlt().x * gfx::g_GFXEngine.GetWidth();
	float height = m_dot.GetSizeFlt().y * gfx::g_GFXEngine.GetHeight();
	m_radius = (width + height) * 0.5f * 0.5f;
	if (index >= 0 && index < vertices.size())
	{
		float u = vertices[index].TexCoord.x;
		float v = vertices[index].TexCoord.y;
		
		m_dot.SetPos(glm::vec2((m_left + m_width * u) - m_radius, (m_top + m_height * v) - m_radius));
	}


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
				if (mPosX < m_left)
				{
					mPosX = m_left;
				}
				if (mPosX > m_right)
				{
					mPosX = m_right;
				}
				if (mPosY < m_top)
				{
					mPosY = m_top;
				}
				if (mPosY > m_bottom)
				{
					mPosY = m_bottom;
				}
				m_dot.SetPos(glm::vec2(mPosX - m_radius, mPosY - m_radius));

				// U
				// V
				std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();
				float u = (mPosX - m_left) / m_width;
				float v = (mPosY - m_top) / m_height;
				vertices[m_index].TexCoord.x = u;
				vertices[m_index].TexCoord.y = v;

				gfx::g_ModelBank.BuildBuffers();
			}
		}
	}
	else
	{
		m_moveableDot = false;
		m_clickedThisFrame = false;
	}
}

void ClickDot::Draw(gfx::RenderQueue* renderQueue)
{
	renderQueue->Enqueue(m_dot);
}

float ClickDot::X()
{
	return m_dot.GetPos().x + m_radius;
}

float ClickDot::Y()
{
	return gfx::g_GFXEngine.GetHeight() - m_dot.GetPos().y + m_radius;
}

float ClickDot::U()
{
	return (X() - m_left) / m_width;
}

float ClickDot::V()
{
	return (Y() - m_top) / m_height;
}

int ClickDot::Index()
{
	return m_index;
}