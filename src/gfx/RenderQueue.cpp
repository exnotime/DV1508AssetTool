#include "RenderQueue.h"

gfx::RenderQueue::RenderQueue(){
	std::vector<gfx::Sprite> list;
	m_SpriteQueue.push_back(list);
	m_SpriteQueue.push_back(list);
	m_SpriteQueue.push_back(list);
	m_TargetTexture = -1;
}

gfx::RenderQueue::~RenderQueue(){

}

void gfx::RenderQueue::Enqueue(const RenderObject& ro) {
	m_Queue.push_back(ro);
}

void gfx::RenderQueue::Enqueue(const Sprite& spr, int layer){
	m_SpriteQueue[layer].push_back(spr);
}

void gfx::RenderQueue::Enqueue(const BrushObject& bo){
	m_BrushQueue.push_back(bo);
}

void gfx::RenderQueue::Enqueue(const GizmoObject& go){
	m_GizmoQueue.push_back(go);
}

void gfx::RenderQueue::Enqueue(const LineObject& lo){
	m_LineQueue.push_back(lo);
}

std::vector<gfx::RenderObject>& gfx::RenderQueue::GetQueue(){
	return m_Queue;
}

std::vector<std::vector<gfx::Sprite>>& gfx::RenderQueue::GetSpriteQueue(){
	return m_SpriteQueue;
}

std::vector<gfx::BrushObject>& gfx::RenderQueue::GetBrushQueue(){
	return m_BrushQueue;
}

std::vector<gfx::GizmoObject>& gfx::RenderQueue::GetGizmoQueue(){
	return m_GizmoQueue;
}

std::vector<gfx::LineObject>& gfx::RenderQueue::GetLineQueue(){
	return m_LineQueue;
}

void gfx::RenderQueue::SetTargetTexture(TextureHandle handle){
	m_TargetTexture = handle;
}

TextureHandle gfx::RenderQueue::GetTargetTexture(){
	return m_TargetTexture;
}

void gfx::RenderQueue::Clear(){
	m_Queue.clear();
	m_BrushQueue.clear();
	m_GizmoQueue.clear();
	m_LineQueue.clear();
	for (auto& it : m_SpriteQueue){
		it.clear();
	}
}