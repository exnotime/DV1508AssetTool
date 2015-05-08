#pragma once
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"
#include "../gfx/Sprite.h"
#include "verticeTranslation/VerticeTranslation.h"
#include "InteractiveArea/InterActiveArea.h"
#include "Button/Button.h"
#include "UV/UVTranslation.h"
#include "InteractiveArea\ModelInteraction.h"
#include "verticeSelection/VerticeSelection.h"
///////////////////////////////////////////////////////////////////////////////
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
	void SetWireFrameModel(gfx::RenderObject ro);
	gfx::RenderObject GetWireFrameModel();
	void Shutdown();
private:
	gfx::ModelHandle	m_Model;
	glm::vec3			m_Pos;
	float				m_Scale;
	float				m_RotateY;
	Button				m_TestButton;
	gfx::Sprite			m_TestSprite;
	TextureHandle		m_TargetTex;
	VerticeTranslation	m_VerticeTranslation;
	VerticeSelection	m_VerticeSelection;
	gfx::RenderObject	m_wfModel;

	InteractiveArea				m_TestArea;
	std::vector<unsigned int>	m_SelectedVertices;
	UVTranslation				m_uvTranslation;


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

