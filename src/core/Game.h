#pragma once
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"
#include "../gfx/Sprite.h"
class Game
{
public:
	Game( );
	~Game( );
	void Initialize();
	void Update(float dt);
	void Render( gfx::RenderQueue* rq );
private:
	gfx::ModelHandle	m_Model;
	glm::vec3			m_Pos;
	float				m_Scale;
	float				m_RotateY;
	gfx::Sprite			m_TestSprite;
};

