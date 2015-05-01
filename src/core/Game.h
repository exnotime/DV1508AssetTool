#pragma once
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"
#include "../gfx/Sprite.h"
#include "verticeTranslation/VerticeTranslation.h"

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
	TextureHandle		m_TargetTex;
	VerticeTranslation			m_VerticeTranslation;
	std::vector<unsigned int>	m_SelectedVertices;
};

