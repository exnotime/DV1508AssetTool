#include "Button.h"
#include "../gfx/RenderQueue.h"
#include <imgui/imgui.h>
Button::Button(){

}

Button::Button(glm::vec2 pos, glm::vec2 size, const char* filename){
	m_Pos = pos;
	m_Size = size;
	m_Sprite.SetTexture(filename);
	m_IntArea.SetPos(pos);
	m_Sprite.SetPos(pos);
	m_IntArea.SetSize(size);
	m_Sprite.SetSize(size);
	m_Clicked = false;
	m_Hovering = false;
}

Button::~Button(){

}

void Button::Update(){
	m_IntArea.Update();
	glm::vec2 pos;
	if (m_Clicked = m_IntArea.IsClicked(pos)){
		m_Sprite.SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
	else{
		m_Sprite.SetColor(glm::vec4(1.0f));
	}
	//check if hovering
	ImGuiIO io = ImGui::GetIO();
	if (io.MousePos.x > m_Pos.x && io.MousePos.y > m_Pos.y && io.MousePos.x < m_Pos.x + m_Size.x && io.MousePos.y < m_Pos.y + m_Size.y){
		m_Hovering = true;
		ImGui::BeginTooltip();
		ImGui::Text(m_Tooltip.c_str());
		ImGui::EndTooltip();
	}
	else{
		m_Hovering = false;
	}

}
void Button::Draw(gfx::RenderQueue* rq){
	rq->Enqueue(m_Sprite);
}
void Button::SetPos(glm::vec2 pos){
	m_Pos = pos;
}
void Button::SetSize(glm::vec2 size){
	m_Size = size;
}
void Button::SetTooltip(const std::string& tooltip){
	m_Tooltip = tooltip;
}
void Button::SetSprite(gfx::Sprite spr){
	m_Sprite = spr;
}
bool Button::IsClicked(){
	return m_Clicked;
}
bool Button::IsHovering(){
	return m_Hovering;
}
