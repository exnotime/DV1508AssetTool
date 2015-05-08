#include "Game.h"
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/MaterialBank.h"
#include "../gfx/Material.h"
#include <nfd/nfd.h>
#include "../gfx/GraphicsEngine.h"
#include "../gfx/Camera.h"
Game::Game( )
{
}


Game::~Game( )
{

}

void Game::Initialize(){
	m_Model = gfx::g_ModelBank.LoadModel("asset/LucinaResource/Lucina_posed.obj");
	m_Pos = glm::vec3(0.0f);
	m_Scale = 1.0f;
	m_RotateY = 0.0f;
	m_VerticeTranslation.Initialize();
	m_uvTranslation.Initialize(m_Model);
	m_VerticeSelection.Initialize();
	m_TestSprite.SetTexture("asset/brush.png");

	m_TestArea.SetPos(glm::vec2(800, 0));
	m_TestArea.SetSize(glm::vec2(800, 900));
	m_TestButton = Button(glm::vec2(0), glm::vec2(50), "asset/brush.png");
	
	///////////////////////////////////////////////////////////////////////////////
	m_TestArea2.Initialize(glm::vec2(640, 720), glm::vec2(0, 0));
	m_AutomaticRotate = false;
	m_AutomaticRotateLeft = false;
	m_MousePos = glm::vec2(0, 0);
	m_PrevMousePos = glm::vec2(0, 0);
	m_Camera = gfx::g_GFXEngine.GetCamera();	
	m_StartPos = m_Camera->GetPosition();
	m_StartScale = m_Camera->GetPosition().z;
	///////////////////////////////////////////////////////////////////////////////
}

void Game::Update(float dt){

	///////////////////////////////////////////////////////////////////////////////
	UpdateModelViewWindow(dt);	
	///////////////////////////////////////////////////////////////////////////////
	m_TestArea.Update();

	// Load model button
	if (ImGui::Button("Load Model"))
	{
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("obj,dae", "", &outPath);
		if (result == NFD_OKAY)
		{
			gfx::g_ModelBank.Clear();
			//gfx::g_MaterialBank.ClearMaterials();
			m_Model = gfx::g_ModelBank.LoadModel(outPath);
			gfx::g_ModelBank.BuildBuffers();
			m_SelectedVertices.clear();
		}
	}

	static float x = 0;
	static float y = 0;
	ImGui::SliderFloat( "X##001", &m_Pos.x, -10, 10 );
	ImGui::SliderFloat( "Y##001", &m_Pos.y, -10, 10 );
	ImGui::SliderFloat( "Z", &m_Pos.z, -20, 10 );
	ImGui::SliderFloat("Scale", &m_Scale, 0, 2);
	ImGui::SliderFloat("RotateY", &m_RotateY, 0, 6.28f);
	m_TestSprite.SetPos(glm::vec2(x, y));

	TempSelectVertices( m_Model, m_SelectedVertices );	// TODO: Remove when real vertice selection is implemented.
	m_VerticeTranslation.SetSelectedVertices( m_SelectedVertices );
	m_VerticeTranslation.Update( dt );
	m_TestButton.Update();

	// UV
	m_uvTranslation.Update(dt);
	SetWireFrameState(m_VerticeSelection.Update(dt));
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(m_Pos) * glm::scale(glm::vec3(m_Scale)) * glm::rotate(m_RotateY, glm::vec3(0, 1, 0));
	
	SetWireFrameModel(ro);

	rq->Enqueue(ro);

	//Set texture
	gfx::Model model = gfx::g_ModelBank.FetchModel(m_Model);
	static int meshIndex = 0;
	ImGui::SliderInt("Mesh Texture", &meshIndex, 0, (int)(model.Meshes.size() - 1));
	//fixing error with imgui when min and max is the same
	if (meshIndex < 0)
		meshIndex = 0;
	gfx::Material* mat = gfx::g_MaterialBank.GetMaterial(model.MaterialOffset + model.Meshes[meshIndex].Material);
	static bool useRoughness = false;
	ImGui::Checkbox("roughness", &useRoughness);
	if (useRoughness){
		rq->SetTargetTexture(mat->GetRoughnessTexture());
	}
	else {
		rq->SetTargetTexture(mat->GetAlbedoTexture());
	}
	m_VerticeTranslation.Draw( rq );
	m_TestButton.Draw(rq);
	m_uvTranslation.Draw(rq);
	m_VerticeSelection.Draw(rq, ro);

	static float brushSize = 24;
	ImGui::SliderFloat("BrushSize", &brushSize, 1, 640);
	glm::vec2 clickPos;
	if (m_TestArea.IsClicked(clickPos)){
		gfx::BrushObject bo;
		bo.Position = clickPos;
		bo.Size = brushSize;
		bo.Texture = m_TestSprite.GetTexture();
		rq->Enqueue(bo);
	}
}

void Game::Shutdown()
{
	m_uvTranslation.Shutdown();
}
void Game::UpdateModelViewWindow(float p_deltaTime)
{
	m_TestArea2.Update();
	float moveSpeed = p_deltaTime * 5.0f;
	float rotationSpeed = p_deltaTime * 2.0f;
	if (!m_TestArea2.GetSpaceState())
	{
		glm::vec3 newPos = glm::vec3(0.0f,0.0f,0.0f);
		if (m_TestArea2.GetWState())
		{
			newPos.z -= moveSpeed;
		}
		if (m_TestArea2.GetAState())
		{
			newPos.x -= moveSpeed;
		}
		if (m_TestArea2.GetSState())
		{
			newPos.z += moveSpeed;
		}
		if (m_TestArea2.GetDState())
		{
			newPos.x += moveSpeed;
		}
		if (m_TestArea2.GetQState())
		{
			newPos.y -= moveSpeed;
		}
		if (m_TestArea2.GetEState())
		{
			newPos.y += moveSpeed;
		}
		m_Camera->MoveRelative(newPos);
	}
	else
	{
		if (m_TestArea2.GetWState())
		{
			m_Camera->PitchRelative(rotationSpeed);
		}
		if (m_TestArea2.GetSState())
		{
			m_Camera->PitchRelative(-rotationSpeed);
		}
		if (m_TestArea2.GetAState())
		{
			m_Camera->YawRelative(rotationSpeed);
		}
		if (m_TestArea2.GetDState())
		{
			m_Camera->YawRelative(-rotationSpeed);
		}
		if (m_TestArea2.GetQState())
		{
			m_Camera->YawRelative(-rotationSpeed);
			glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
			newPos.x -= moveSpeed;
			m_Camera->MoveRelative(newPos);
		}
		if (m_TestArea2.GetEState())
		{
			m_Camera->YawRelative(rotationSpeed);
			glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
			newPos.x += moveSpeed;
			m_Camera->MoveRelative(newPos);
		}
	}
}

gfx::RenderObject Game::GetWireFrameModel(){
	return m_wfModel;
}

void Game::SetWireFrameModel(gfx::RenderObject ro){
	m_wfModel = ro;
}

bool Game::GetWireFrameState(){
	return m_wfState;
}

void Game::SetWireFrameState(bool state){
	m_wfState = state;
}