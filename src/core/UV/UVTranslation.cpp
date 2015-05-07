#include "UVTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

//UVTranslation::UVTranslation(){}
//UVTranslation::~UVTranslation(){}

void UVTranslation::Initialize(gfx::ModelHandle modelHandle)
{
	m_uvModeOn = true;

	const gfx::Model& model = gfx::g_ModelBank.FetchModel(modelHandle);
	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	// 0 {1 1}
	// 1 {0 0}
	// 2 {0 1}

	//std::vector<gfx::VertexPosNormalTexTangent>& vertices2 = gfx::g_ModelBank.GetVertices();

	// 1
	m_uvPos1 = new float[2];
	m_uvPos1[0] = vertices[0].TexCoord.x;
	m_uvPos1[1] = vertices[0].TexCoord.y;

	m_dotSprite1.SetTexture("asset/Cube/rDot.png");
	m_dotSprite1.SetPos(glm::vec2(800.0f, 0.0f));
	
	// 2
	m_uvPos2 = new float[2];
	m_uvPos2[0] = vertices[1].TexCoord.x;
	m_uvPos2[1] = vertices[1].TexCoord.y;

	m_dotSprite2.SetTexture("asset/Cube/gDot.png");
	m_dotSprite2.SetPos(glm::vec2(800.0f, 0.0f));

	// 3
	m_uvPos3 = new float[2];
	m_uvPos3[0] = vertices[2].TexCoord.x;
	m_uvPos3[1] = vertices[2].TexCoord.y;

	m_dotSprite3.SetTexture("asset/Cube/bDot.png");
	m_dotSprite3.SetPos(glm::vec2(800.0f, 0.0f));

	// Temp (remove later)
	// 4
	m_uvPos4 = new float[2];
	m_uvPos4[0] = vertices[3].TexCoord.x;
	m_uvPos4[1] = vertices[3].TexCoord.y;

	m_dotSprite4.SetTexture("asset/Cube/rDot.png");
	m_dotSprite4.SetPos(glm::vec2(800.0f, 0.0f));

}

void UVTranslation::Update(const float deltaTime)
{

	ImGui::Begin("UV");

	ImGui::Checkbox("Enable", &m_uvModeOn);

	// 1
	ImGui::SliderFloat2("UV Red", m_uvPos1, 0.0f, 1.0f);
	m_dotSprite1.SetPos(glm::vec2(800.0f + m_uvPos1[0] * 800.0f - 7.5f, m_uvPos1[1] * 900.0f - 7.5f));

	// 2
	ImGui::SliderFloat2("UV Green", m_uvPos2, 0.0f, 1.0f);
	m_dotSprite2.SetPos(glm::vec2(800.0f + m_uvPos2[0] * 800.0f - 7.5f, m_uvPos2[1] * 900.0f - 7.5f));

	// 3
	ImGui::SliderFloat2("UV Blue", m_uvPos3, 0.0f, 1.0f);
	m_dotSprite3.SetPos(glm::vec2(800.0f + m_uvPos3[0] * 800.0f - 7.5f, m_uvPos3[1] * 900.0f - 7.5f));

	// remove 4 later
	// 4
	ImGui::SliderFloat2("UV Red2", m_uvPos4, 0.0f, 1.0f);
	m_dotSprite4.SetPos(glm::vec2(800.0f + m_uvPos4[0] * 800.0f - 7.5f, m_uvPos4[1] * 900.0f - 7.5f));

	if (m_uvModeOn)
	{
		std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

		vertices[0].TexCoord.x = m_uvPos1[0];
		vertices[0].TexCoord.y = m_uvPos1[1];

		vertices[1].TexCoord.x = m_uvPos2[0];
		vertices[1].TexCoord.y = m_uvPos2[1];

		vertices[2].TexCoord.x = m_uvPos3[0];
		vertices[2].TexCoord.y = m_uvPos3[1];

		// remove later
		vertices[3].TexCoord.x = m_uvPos4[0];
		vertices[3].TexCoord.y = m_uvPos4[1];
	}

	ImGui::End();
}

void UVTranslation::Draw(gfx::RenderQueue* renderQueue)
{
	if (m_uvModeOn)
	{
		renderQueue->Enqueue(m_dotSprite1);
		renderQueue->Enqueue(m_dotSprite2);
		renderQueue->Enqueue(m_dotSprite3);

		// remove later
		renderQueue->Enqueue(m_dotSprite4);
	}
}

void UVTranslation::Shutdown()
{
	delete[] m_uvPos1;
	delete[] m_uvPos2;
	delete[] m_uvPos3;

	delete[] m_uvPos4;
}