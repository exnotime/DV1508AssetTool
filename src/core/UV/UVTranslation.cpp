#include "UVTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

//UVTranslation::UVTranslation(){}
//UVTranslation::~UVTranslation(){}

void UVTranslation::Initialize(int winWidth, int winHeight)
{
	m_uvModeOn = false;

	m_winWidth = winWidth;
	m_winHeight = winHeight;

	m_dots = std::vector<ClickDot>();

	m_selectedFace = glm::vec3(0, 1, 2);

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

	ImGui::Begin("UV");

	ImGui::Checkbox("Enable", &m_uvModeOn);

	ImGui::End();


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
		cd.Initialize(vertex, m_winWidth, m_winHeight, "asset/Cube/rDot.png");
		
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