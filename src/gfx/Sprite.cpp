#include "Sprite.h"
#include "GraphicsEngine.h"
gfx::Sprite::Sprite(){
	m_Pos = glm::vec4(0,1,0,0);
	m_Size = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

gfx::Sprite::~Sprite(){
}

gfx::Sprite::Sprite(const char* filename){
	m_Texture = g_MaterialBank.LoadTexture(filename);

	float width = g_MaterialBank.GetTexture(m_Texture)->GetWidth() / g_GFXEngine.GetWidth();
	float height = g_MaterialBank.GetTexture(m_Texture)->GetHeight() / g_GFXEngine.GetHeight();
	m_Pos = glm::vec4(0);
	m_Size = glm::vec4(width, height, 1.0f, 1.0f);
}

void gfx::Sprite::SetPos(const glm::vec2& pos){
	m_Pos.x = pos.x / g_GFXEngine.GetWidth();
	m_Pos.y = 1.0f - (pos.y / g_GFXEngine.GetHeight());
}
void gfx::Sprite::SetSize(const glm::vec2& size){
	m_Size.x = size.x / g_GFXEngine.GetWidth();
	m_Size.y = size.y / g_GFXEngine.GetHeight();
}
void gfx::Sprite::SetTexture(const char* filename){
	m_Texture = g_MaterialBank.LoadTexture(filename);

	float width = g_MaterialBank.GetTexture(m_Texture)->GetWidth() / g_GFXEngine.GetWidth();
	float height = g_MaterialBank.GetTexture(m_Texture)->GetHeight() / g_GFXEngine.GetHeight();
	m_Size = glm::vec4(width, height, 1.0f, 1.0f);
}
void gfx::Sprite::SetTexture(TextureHandle tex){
	m_Texture = tex;
	float width = g_MaterialBank.GetTexture(m_Texture)->GetWidth() / g_GFXEngine.GetWidth();
	float height = g_MaterialBank.GetTexture(m_Texture)->GetHeight() / g_GFXEngine.GetHeight();
	m_Size = glm::vec4(width, height, 1.0f, 1.0f);
}
glm::vec2& gfx::Sprite::GetPos(){
	return glm::vec2(m_Pos.x * g_GFXEngine.GetWidth(), m_Pos.y * g_GFXEngine.GetHeight());
}
glm::vec2& gfx::Sprite::GetSize(){
	return glm::vec2(m_Size.x * g_GFXEngine.GetWidth(), m_Size.y * g_GFXEngine.GetHeight());
}
glm::vec4& gfx::Sprite::GetPosFlt(){
	return m_Pos;
}
glm::vec4& gfx::Sprite::GetSizeFlt(){
	return m_Size;
}
TextureHandle gfx::Sprite::GetTexture(){
	return m_Texture;
}

