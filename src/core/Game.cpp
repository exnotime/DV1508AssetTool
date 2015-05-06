#include "Game.h"
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/MaterialBank.h"
#include "../gfx/Material.h"
Game::Game( )
{
}


Game::~Game( )
{

}

void Game::Initialize(){
	m_TargetTex = gfx::g_MaterialBank.LoadTexture("asset/flcl.jpg", gfx::TEXTURE_COLOR);
	m_Model = gfx::g_ModelBank.LoadModel("asset/LucinaResource/Lucina_posed.obj");
	m_Pos = glm::vec3(0.0f);
	m_Scale = 1.0f;
	m_RotateY = 0.0f;
	m_VerticeTranslation.Initialize();
	m_TestSprite.SetTexture("asset/brush.png");
	m_TargetTex = gfx::g_MaterialBank.LoadTexture("asset/flcl.jpg",gfx::TEXTURE_COLOR);

	m_TestArea.SetPos(glm::vec2(640, 0));
	m_TestArea.SetSize(glm::vec2(640, 720));

	///////////////////////////////////////////////////////////////////////////////
	m_TestArea2.Initialize(glm::vec2(640, 720), glm::vec2(0, 0));
	m_AutomaticRotate = false;
	m_MousePos = glm::vec2(0, 0);
	m_PrevMousePos = glm::vec2(0, 0);
	m_StartPos = m_Pos;
	m_StartRotationY = m_RotateY;
	m_StartScale = m_Scale;
	m_AutomaticRotateLeft = false;
	///////////////////////////////////////////////////////////////////////////////
}

void Game::Update(float dt){

	///////////////////////////////////////////////////////////////////////////////

	m_PrevMousePos = m_MousePos;
	m_TestArea2.Update();
	m_MousePos = m_TestArea2.GetMousePos();
	//Rotate the model around Y
	if (m_TestArea2.GetLeftMousePressed())
	{
		float x = 0, y = 0;
		x = m_PrevMousePos.x - m_MousePos.x;
		//y = m_PrevMousePos.y - m_MousePos.y;
		if (x < 0)
		{
			m_AutomaticRotateLeft = false;
		}
		else
		{
			m_AutomaticRotateLeft = true;
		}
		m_RotateY -= x * dt;
		//m_Pos.y += y * dt;
	}
	//Move the model around in the viewspace
	if (m_TestArea2.GetRightMousePressed())
	{
		float x = 0, y = 0;
		x = m_PrevMousePos.x - m_MousePos.x;
		y = m_PrevMousePos.y - m_MousePos.y;
		m_Pos.x -= x * dt;
		m_Pos.y += y * dt;
	}
	//if the Y rotation goes past min/max then reset
	if (m_RotateY > 6.28f)
	{
		m_RotateY = 0.0f;
	}
	if (m_RotateY < -6.28f)
	{
		m_RotateY = 0.0f;
	}
	//Manipulate scale by scrolling the mouse wheel
	m_Scale += m_TestArea2.GetMouseWheelState() * dt;

	//Reset position, rotation and scale
	if (m_TestArea2.GetMouseWheelClicked())
	{
		m_Scale = m_StartScale;
	}

	if (m_TestArea2.GetRightMouseDoubleClicked())
	{
		m_Pos = m_StartPos;
	}

	if (m_TestArea2.GetLeftMouseDoubleClicked() && m_RotateY == m_StartRotationY && !m_AutomaticRotate)
	{
		m_AutomaticRotate = true;
	}
	else if (m_TestArea2.GetLeftMouseDoubleClicked() && m_AutomaticRotate)
	{
		m_AutomaticRotate = false;
	}
	else if (m_TestArea2.GetLeftMouseDoubleClicked())
	{
		m_RotateY = m_StartRotationY;
	}
	//Rotates automatically
	if (m_AutomaticRotate)
	{
		if (!m_AutomaticRotateLeft)
		{
			m_RotateY += dt;
		}
		else
		{
			m_RotateY -= dt;
		}
	}
	///////////////////////////////////////////////////////////////////////////////
	m_TestArea.Update();
	static float x = 0;
	static float y = 0;
	ImGui::SliderFloat( "X##001", &m_Pos.x, -10, 10 );
	ImGui::SliderFloat( "Y##001", &m_Pos.y, -10, 10 );
	ImGui::SliderFloat( "Z", &m_Pos.z, -20, 10 );
	ImGui::SliderFloat("Scale", &m_Scale, 0, 2);
	ImGui::SliderFloat("RotateY", &m_RotateY, 0, 6.28f);
	m_TestSprite.SetPos(glm::vec2(x, y));

	TempSelectVertices( m_Model, m_SelectedVertices );	// TODO: Remove when real vertice selection is implemented.
	m_VerticeTranslation.SetSelectedVertices( m_SelectedVertices );
	m_VerticeTranslation.Update( dt );
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(m_Pos) * glm::scale(glm::vec3(m_Scale)) * glm::rotate(m_RotateY, glm::vec3(0, 1, 0));
	rq->Enqueue(ro);

	//Set texture
	gfx::Model lucina = gfx::g_ModelBank.FetchModel(m_Model);
	static int meshIndex = 0;
	ImGui::SliderInt("Mesh Texture", &meshIndex, 0, lucina.Meshes.size() - 1);
	gfx::Material* lucMat = gfx::g_MaterialBank.GetMaterial(lucina.Meshes[meshIndex].Material);
	rq->SetTargetTexture(lucMat->GetAlbedoTexture());

	m_VerticeTranslation.Draw( rq );

	static float brushSize = 24;
	ImGui::SliderFloat("BrushSize", &brushSize, 1, 640);
	glm::vec2 clickPos;
	if (m_TestArea.IsClicked(clickPos)){
		gfx::BrushObject bo;
		bo.Position = clickPos;
		bo.Size = brushSize;
		bo.Texture = m_TestSprite.GetTexture();
		rq->Enqueue(bo);
	}

}