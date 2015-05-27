#include "VerticeSelection.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

VerticeSelection::VerticeSelection(){

}

VerticeSelection::~VerticeSelection(){

}

void VerticeSelection::Initialize(){
	m_Checked = false;


}

bool VerticeSelection::Update(const float deltaTime){
	ImGui::Begin("Vertex Selection");

	ImGui::Checkbox("Enabled", &m_Checked);

	ImGui::End();

	return m_Checked;
}

void VerticeSelection::Draw(gfx::RenderQueue* renderQueue, gfx::RenderObject ro){

}