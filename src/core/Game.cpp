#include "Game.h"
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
Game::Game( )
{
}


Game::~Game( )
{

}

void Game::Initialize(){
	m_Model = gfx::g_ModelBank.LoadModel("asset/shy guy/shyguy.dae");
	m_Pos = glm::vec3(0,-1,-10);
	m_Scale = 1.0f;
	m_RotateY = 0.0f;
}

void Game::Update(float dt){
	
	ImGui::SliderFloat( "X", &m_Pos.x, -3, 3 );
	ImGui::SliderFloat( "Y", &m_Pos.y, -3, 3 );
	ImGui::SliderFloat( "Z", &m_Pos.z, -20, -2 );
	ImGui::SliderFloat( "Scale", &m_Scale, 0, 3 );
	ImGui::SliderFloat("RotateY", &m_RotateY, 0, 6.28f);
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(m_Pos) * glm::scale(glm::vec3(m_Scale)) * glm::rotate(m_RotateY, glm::vec3(0, 1, 0)) * glm::rotate(1.57f, glm::vec3(1, 0, 0));
	
	rq->Enqueue(ro);
}