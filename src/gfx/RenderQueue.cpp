#include "RenderQueue.h"

gfx::RenderQueue::RenderQueue(){
	std::vector<gfx::Sprite> list;
	m_SpriteQueue.push_back(list);
	m_SpriteQueue.push_back(list);
	m_SpriteQueue.push_back(list);
	m_TargetTexture = -1;
}

gfx::RenderQueue::~RenderQueue( ){

}

void gfx::RenderQueue::Enqueue( const RenderObject& ro ) {
	m_Queue.push_back( ro ); 
}
void gfx::RenderQueue::Enqueue(const Sprite& spr, int layer){
	m_SpriteQueue[layer].push_back(spr);
}
std::vector<gfx::RenderObject>& gfx::RenderQueue::GetQueue( ){ 
	return m_Queue;
}

std::vector<std::vector<gfx::Sprite>>& gfx::RenderQueue::GetSpriteQueue(){
	return m_SpriteQueue;
}

void gfx::RenderQueue::SetTargetTexture(TextureHandle handle){
	m_TargetTexture = handle;
}

TextureHandle gfx::RenderQueue::GetTargetTexture(){
	return m_TargetTexture;
}

void gfx::RenderQueue::Clear( ){
	m_Queue.clear();
	for (auto& it : m_SpriteQueue){
		it.clear();
	}
}