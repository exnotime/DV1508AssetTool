#include "RenderQueue.h"

gfx::RenderQueue::RenderQueue(){
	
}

gfx::RenderQueue::~RenderQueue( ){

}

void gfx::RenderQueue::Enqueue( const RenderObject& ro ) {
	m_Queue.push_back( ro ); 
}

std::vector<gfx::RenderObject>& gfx::RenderQueue::GetQueue( ){ 
	return m_Queue;
}

void gfx::RenderQueue::Clear( ){
	m_Queue.clear();
}