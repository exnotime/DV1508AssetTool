#include "InterActiveArea.h"
#include <imgui/imgui.h>
InteractiveArea::InteractiveArea(){

}

InteractiveArea::~InteractiveArea(){

}

bool InteractiveArea::IsClicked(glm::vec2& outPos){
	if (m_ClickedThisFrame){
		outPos = m_ClickPos;
		return true;
	}
	outPos = glm::vec2(-1);
	return false;
}

void InteractiveArea::Update(){
	m_ClickedLastFrame = m_ClickedThisFrame;
	ImGuiIO& io = ImGui::GetIO();
	//0 is left button
	if (io.MouseClicked[0]){
		ImVec2 mousePos = io.MouseClickedPos[0];
		if (mousePos.x > m_Position.x && mousePos.x < m_Position.x + m_Size.x){
			if (mousePos.y > m_Position.y && mousePos.y < m_Position.y + m_Size.y){
				m_ClickedThisFrame = true;
				glm::vec2 translatedPos = glm::vec2(mousePos.x - m_Position.x, mousePos.y - m_Position.y);
				m_ClickPos = translatedPos / m_Size;
			}
		}
	}
	else {
		m_ClickedThisFrame = false;
	}
}

void InteractiveArea::SetSize(const glm::vec2& size){
	m_Size = size;
}
void InteractiveArea::SetPos(const glm::vec2& pos){
	m_Position = pos;
}
glm::vec2 InteractiveArea::GetSize(){
	return m_Size;
}
glm::vec2 InteractiveArea::GetPos(){
	return m_Position;
}