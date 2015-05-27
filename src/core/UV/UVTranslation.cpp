#include "UVTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "GraphicsEngine.h"

//UVTranslation::UVTranslation(){}
//UVTranslation::~UVTranslation(){}

void UVTranslation::Initialize(int winWidth, int winHeight)
{
	m_uvModeOn = false;

	m_winWidth = winWidth;
	m_winHeight = winHeight;

	m_dots = std::vector<ClickDot>();

	//const gfx::Model& model = gfx::g_ModelBank.FetchModel(modelHandle);
	//std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	AddVertex(0);
	AddVertex(1);
	AddVertex(2);
	// 0 {1 1}
	// 1 {0 0}
	// 2 {0 1}
	
}

void UVTranslation::Update(const float deltaTime)
{
	if (m_uvModeOn)
	{
		std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

		for (unsigned int i = 0; i < m_dots.size(); i++)
		{
			m_dots[i].Update(deltaTime);
		}
	}
}

void UVTranslation::Draw(gfx::RenderQueue* renderQueue)
{
	if (m_uvModeOn)
	{
		std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

		for (unsigned int i = 0; i < m_dots.size(); i++)
		{
			m_dots[i].Draw(renderQueue);
		}

		gfx::LineObject lo;
		lo.Lines = GetLinePositions();
		lo.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		renderQueue->Enqueue(lo);
	}
}

void UVTranslation::Shutdown()
{
}

void UVTranslation::AddVertex(int vertex)
{
	if (!ListContains(vertex))
	{
		ClickDot cd = ClickDot();
		cd.Initialize(vertex, m_winWidth, m_winHeight, "asset/Cube/rDot3.png");
		
		m_dots.push_back(cd);
	}
}

void UVTranslation::RemoveVertex(int vertex)
{
	for (unsigned int i = 0; i < m_dots.size(); i++)
	{
		if (m_dots[i].Index() == vertex)
		{
			m_dots.erase(m_dots.begin() + i);
			break;
		}
	}
}

bool UVTranslation::ListContains(int vertex)
{
	for (unsigned int i = 0; i < m_dots.size(); i++)
	{
		if (m_dots[i].Index() == vertex)
		{
			return true;
		}
	}

	return false;
}

std::vector<glm::vec2> UVTranslation::GetLinePositions()
{
	std::vector<glm::vec2> positions = std::vector<glm::vec2>();

	for (unsigned int i = 0; i < m_dots.size(); i++)
	{
		glm::vec2 pos = glm::vec2(m_dots[i].X(), gfx::g_GFXEngine.GetHeight() - m_dots[i].Y());
		
		glm::vec2 pos2;
		if (i >= m_dots.size() - 1)
		{
			pos2 = glm::vec2(m_dots[0].X(), gfx::g_GFXEngine.GetHeight() - m_dots[0].Y());
		}
		else
		{
			pos2 = glm::vec2(m_dots[i + 1].X(), gfx::g_GFXEngine.GetHeight() - m_dots[i + 1].Y());
		}

		positions.push_back(pos);
		positions.push_back(pos2);
	}

	return positions;
}

void UVTranslation::ResetList()
{
	m_dots.clear();

	AddVertex(0);
	AddVertex(1);
	AddVertex(2);
}

void UVTranslation::Enable()
{
	m_uvModeOn = true;
}

void UVTranslation::Disable()
{
	m_uvModeOn = false;
}

void UVTranslation::Toggle()
{
	m_uvModeOn = !m_uvModeOn;
}