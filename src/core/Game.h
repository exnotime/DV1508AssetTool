#pragma once
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"
#include "../gfx/Sprite.h"
#include "verticeTranslation/VerticeTranslation.h"
#include "InteractiveArea/InterActiveArea.h"
#include "Brush/BrushArea.h"
#include "Button/Button.h"
#include "UV/UVTranslation.h"
#include "InteractiveArea\ModelInteraction.h"
#include "verticeSelection/VerticeSelection.h"
#include "Brush/BrushTexGenerator.h"
#include "ColorPicker\ColorPicker.h"
///////////////////////////////////////////////////////////////////////////////
class Camera;
///////////////////////////////////////////////////////////////////////////////

class Game
{
public:
	Game( );
	~Game( );
	void Initialize(int width, int height);
	void Update(float dt);
	void Render( gfx::RenderQueue* rq );
	void SetWireFrameModel(gfx::RenderObject ro);
	gfx::RenderObject GetWireFrameModel();
	void Shutdown();

	bool GetWireFrameState();
	void SetWireFrameState(bool state);
private:
	gfx::ModelHandle			m_Model;
	gfx::Sprite					m_TestSprite;
	gfx::Sprite					m_BrushGhost;
	TextureHandle				m_TargetTex;
	VerticeTranslation			m_VerticeTranslation;
	VerticeSelection			m_VerticeSelection;
	gfx::RenderObject			m_wfModel;
	BrushArea					m_BrushArea;
	InteractiveArea				m_TestArea;
	std::vector<unsigned int>	m_SelectedVertices;
	UVTranslation				m_uvTranslation;
	BrushTexGenerator			m_BrushGenerator;
	ColorPicker			m_colorPicker;

	Button						m_LoadModelButton;

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

	bool m_wfState;
};

