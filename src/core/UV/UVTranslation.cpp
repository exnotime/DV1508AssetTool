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
	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	// 0 {1 1}
	// 1 {0 0}
	// 2 {0 1}

	/*
	// 1
	m_uvPos1 = new float[2];
	m_uvPos1[0] = vertices[(int)m_selectedFace.x].TexCoord.x;
	m_uvPos1[1] = vertices[(int)m_selectedFace.x].TexCoord.y;

	m_dotSprite1.SetTexture("asset/Cube/rDot.png");
	m_dotSprite1.SetPos(glm::vec2(800.0f, 0.0f));
	
	// 2
	m_uvPos2 = new float[2];
	m_uvPos2[0] = vertices[(int)m_selectedFace.y].TexCoord.x;
	m_uvPos2[1] = vertices[(int)m_selectedFace.y].TexCoord.y;

	m_dotSprite2.SetTexture("asset/Cube/gDot.png");
	m_dotSprite2.SetPos(glm::vec2(800.0f, 0.0f));

	// 3
	m_uvPos3 = new float[2];
	m_uvPos3[0] = vertices[(int)m_selectedFace.z].TexCoord.x;
	m_uvPos3[1] = vertices[(int)m_selectedFace.z].TexCoord.y;

	m_dotSprite3.SetTexture("asset/Cube/bDot.png");
	m_dotSprite3.SetPos(glm::vec2(800.0f, 0.0f));

	// Temp (remove later)
	// 4
	m_uvPos4 = new float[2];
	m_uvPos4[0] = vertices[3].TexCoord.x;
	m_uvPos4[1] = vertices[3].TexCoord.y;

	m_dotSprite4.SetTexture("asset/Cube/rDot.png");
	m_dotSprite4.SetPos(glm::vec2(800.0f, 0.0f));
	*/

}

void UVTranslation::Update(const float deltaTime)
{

	ImGui::Begin("UV");

	ImGui::Checkbox("Enable", &m_uvModeOn);

	ImGui::End();

	/*
	// 1
	ImGui::SliderFloat2("UV Red", m_uvPos1, 0.0f, 1.0f);
	m_dotSprite1.SetPos(glm::vec2(800.0f + m_uvPos1[0] * 800.0f - 7.5f, m_uvPos1[1] * 900.0f - 7.5f));

	// 2
	ImGui::SliderFloat2("UV Green", m_uvPos2, 0.0f, 1.0f);
	m_dotSprite2.SetPos(glm::vec2(800.0f + m_uvPos2[0] * 800.0f - 7.5f, m_uvPos2[1] * 900.0f - 7.5f));

	// 3
	ImGui::SliderFloat2("UV Blue", m_uvPos3, 0.0f, 1.0f);
	m_dotSprite3.SetPos(glm::vec2(800.0f + m_uvPos3[0] * 800.0f - 7.5f, m_uvPos3[1] * 900.0f - 7.5f));

	// temp
	// 4
	ImGui::SliderFloat2("UV Red2", m_uvPos4, 0.0f, 1.0f);
	m_dotSprite4.SetPos(glm::vec2(800.0f + m_uvPos4[0] * 800.0f - 7.5f, m_uvPos4[1] * 900.0f - 7.5f));
	*/

	if (m_uvModeOn)
	{
		std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

		/*
		vertices[(int)m_selectedFace.x].TexCoord.x = m_uvPos1[0];
		vertices[(int)m_selectedFace.x].TexCoord.y = m_uvPos1[1];

		vertices[(int)m_selectedFace.y].TexCoord.x = m_uvPos2[0];
		vertices[(int)m_selectedFace.y].TexCoord.y = m_uvPos2[1];

		vertices[(int)m_selectedFace.z].TexCoord.x = m_uvPos3[0];
		vertices[(int)m_selectedFace.z].TexCoord.y = m_uvPos3[1];

		// temp
		vertices[3].TexCoord.x = m_uvPos4[0];
		vertices[3].TexCoord.y = m_uvPos4[1];
		*/

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
		/*
		renderQueue->Enqueue(m_dotSprite1);
		renderQueue->Enqueue(m_dotSprite2);
		renderQueue->Enqueue(m_dotSprite3);

		// temp
		renderQueue->Enqueue(m_dotSprite4);
		*/
		for (unsigned int i = 0; i < m_dots.size(); i++)
		{
			m_dots[i].Draw(renderQueue);
		}
	}
}

void UVTranslation::Shutdown()
{
	/*
	delete[] m_uvPos1;
	delete[] m_uvPos2;
	delete[] m_uvPos3;

	delete[] m_uvPos4;
	*/
}

//void UVTranslation::SelectFace(int vertex1, int vertex2, int vertex3)
//{
//	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();
//
//	if (vertex1 >= 0 && vertex1 < vertices.size()
//		&& vertex2 >= 0 && vertex2 < vertices.size()
//		&& vertex3 >= 0 && vertex3 < vertices.size())
//	{
//		m_selectedFace.x = (float)vertex1;
//		m_selectedFace.y = (float)vertex2;
//		m_selectedFace.z = (float)vertex3;
//
//		/*
//		m_uvPos1[0] = vertices[(int)m_selectedFace.x].TexCoord.x;
//		m_uvPos1[1] = vertices[(int)m_selectedFace.x].TexCoord.y;
//
//		m_uvPos2[0] = vertices[(int)m_selectedFace.y].TexCoord.x;
//		m_uvPos2[1] = vertices[(int)m_selectedFace.y].TexCoord.y;
//
//		m_uvPos3[0] = vertices[(int)m_selectedFace.z].TexCoord.x;
//		m_uvPos3[1] = vertices[(int)m_selectedFace.z].TexCoord.y;
//		*/
//	}
//}

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