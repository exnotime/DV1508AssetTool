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
	m_Model = gfx::g_ModelBank.LoadModel("asset/LucinaResource/Lucina_posed.obj");
	m_Pos = glm::vec3(0,-1,-10);
	m_Scale = 1.0f;
	m_RotateY = 0.0f;
	m_TestSprite.SetTexture("asset/rockman_teeth.png");
}

void Game::Update(float dt){
	static float x = 0;
	static float y = 0;
	ImGui::SliderFloat( "X##001", &m_Pos.x, -3, 3 );
	ImGui::SliderFloat( "Y##001", &m_Pos.y, -3, 3 );
	ImGui::SliderFloat( "Z", &m_Pos.z, -20, -2 );
	ImGui::SliderFloat( "Scale", &m_Scale, 0, 3 );
	ImGui::SliderFloat("RotateY", &m_RotateY, 0, 6.28f);
	ImGui::Text("SpriteOptions");
	ImGui::SliderFloat("X##002", &x, 0, 1600);
	ImGui::SliderFloat("Y##002", &y, 0, 900);
	m_TestSprite.SetPos(glm::vec2(x, y));
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(m_Pos) * glm::scale(glm::vec3(m_Scale)) * glm::rotate(m_RotateY, glm::vec3(0, 1, 0));
	
	rq->Enqueue(ro);
	rq->Enqueue(m_TestSprite);
}