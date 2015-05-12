#include "BrushArea.h"
#include <imgui/imgui.h>
BrushArea::BrushArea(){
	m_IsActive = false;
	m_MousePos = glm::vec2(0, 0);
}

BrushArea::~BrushArea(){

}

void BrushArea::Update(){
	m_Area.Update();
	glm::vec2 clickpos;
	ImGuiIO& io = ImGui::GetIO();
	if (m_Area.IsClicked(clickpos)){
		m_IsActive = true;
		m_MousePos = glm::vec2(io.MousePos.x, io.MousePos.y);

		gfx::BrushObject bo;
		bo.Position = clickpos;
		bo.Texture = m_BrushTexture;
		bo.Size = m_BrushSize;
		m_BrushObjects.push_back(bo);
	}
	
	if (m_IsActive){
		if (!io.MouseDown[0]){
			m_IsActive = false;
			return;
		}

		clickpos = glm::vec2(io.MousePos.x, io.MousePos.y);
		glm::vec2 deltaVector = clickpos - m_MousePos;
		float deltaLength = glm::length(deltaVector);
		deltaVector /= deltaLength; //normalize

		if (deltaLength > BRUSH_DELTA * m_BrushSize){

			gfx::BrushObject bo;
			int numBrush = deltaLength / (BRUSH_DELTA * m_BrushSize);
			int i = 0;
			for (; i < numBrush; i++){
				glm::vec2 deltaPos = m_MousePos + deltaVector *(i * BRUSH_DELTA * m_BrushSize);

				glm::vec2 translatedPos = glm::vec2(deltaPos.x - m_Area.GetPos().x, deltaPos.y - m_Area.GetPos().y);
				glm::vec2 brushpos = translatedPos / m_Area.GetSize();

				if (deltaLength > 30)
					int k = 0;

				bo.Position = brushpos;
				bo.Texture = m_BrushTexture;
				bo.Size = m_BrushSize;
				m_BrushObjects.push_back(bo);
			}
			m_MousePos += deltaVector * deltaLength;
		}
	}
}

float BrushArea::GetBrushSize(){
	return m_BrushSize;
}

void BrushArea::SetBrushSize(float size){
	m_BrushSize = size;
}

void BrushArea::PushStrokes(gfx::RenderQueue* rq){
	for (auto& brush : m_BrushObjects){
		rq->Enqueue(brush);
	}
	m_BrushObjects.clear();
}

InteractiveArea& BrushArea::GetArea(){
	return m_Area;
}

void BrushArea::SetArea(const InteractiveArea& area){
	m_Area = area;
}

TextureHandle BrushArea::GetTexture(){
	return m_BrushTexture;
}

void BrushArea::SetTexture(TextureHandle handle){
	m_BrushTexture = handle;
}