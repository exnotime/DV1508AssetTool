#include "Button.h"
#include "../gfx/RenderQueue.h"
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
}

Button::~Button(){

}

void Button::Update(){
	m_IntArea.Update();
	glm::vec2 pos;
	if (m_IntArea.IsClicked(pos)){
		m_Sprite.SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
	else{
		m_Sprite.SetColor(glm::vec4(1.0f));
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