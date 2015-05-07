#pragma once
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"
#include "../gfx/Sprite.h"
#include "verticeTranslation/VerticeTranslation.h"
#include "InteractiveArea/InterActiveArea.h"

///////////////////////////////////////////////////////////////////////////////
#include "InteractiveArea\ModelInteraction.h"
class Camera;
///////////////////////////////////////////////////////////////////////////////
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
	InteractiveArea				m_TestArea;
	std::vector<unsigned int>	m_SelectedVertices;



	///////////////////////////////////////////////////////////////////////////////
	void UpdateModelViewWindow(float p_deltaTime);

	Camera* m_Camera;
	glm::vec3			m_StartPos;
	ModelInteraction 				m_TestArea2;
	glm::vec2 m_clickedPos;
	//float m_StartRotationY;
	float m_StartScale;
	bool m_AutomaticRotate;
	bool m_AutomaticRotateLeft;

	glm::vec2 m_MousePos;
	glm::vec2 m_PrevMousePos;

	///////////////////////////////////////////////////////////////////////////////

};

