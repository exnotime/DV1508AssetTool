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

void Game::Initialize(int width, int height){
	m_Model = gfx::g_ModelBank.LoadModel("asset/Bomb/model.obj");
	m_VerticeTranslation.Initialize();
	m_uvTranslation.Initialize(width, height);
	m_VerticeSelection.Initialize();
	m_TestSprite.SetTexture("asset/brush.png");
	m_BrushGhost.SetTexture("asset/BrushGhost.png");
	m_TestArea.SetPos(glm::vec2(width / 2, 0));
	m_TestArea.SetSize(glm::vec2(width / 2 , height));

	m_BrushArea.SetArea(m_TestArea);
	m_BrushArea.SetBrushSize(64);
	m_BrushArea.SetTexture(m_TestSprite.GetTexture());

	m_BrushGenerator.Init();
	m_BrushGenerator.GenerateTexture(64, 0.5f, m_TestSprite.GetTexture());

	m_LoadModelButton = Button(glm::vec2(0, 300), glm::vec2(50, 50), "asset/Icons/S_Load_Model.png");
	m_LoadModelButton.SetTooltip("Load Model");
	///////////////////////////////////////////////////////////////////////////////
	m_TestArea2.Initialize(glm::vec2(width / 2, height), glm::vec2(0, 0));
	m_AutomaticRotate = false;
	m_AutomaticRotateLeft = false;
	m_MousePos = glm::vec2(0, 0);
	m_PrevMousePos = glm::vec2(0, 0);
	m_Camera = gfx::g_GFXEngine.GetCamera();
	m_Camera->MoveWorld(glm::vec3(0, -8, -15));
	m_StartPos = m_Camera->GetPosition();
	m_StartScale = m_Camera->GetPosition().z;
	///////////////////////////////////////////////////////////////////////////////
}

void Game::Update(float dt){

	///////////////////////////////////////////////////////////////////////////////
	UpdateModelViewWindow(dt);	
	///////////////////////////////////////////////////////////////////////////////
	m_BrushArea.Update();
	m_LoadModelButton.Update();
	// Load model button
	if (m_LoadModelButton.IsClicked())
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
	static float h = 0;
	ImGui::SliderFloat("Hardness", &h, 0, 1);
	if (ImGui::Button("GenBrush")){
		m_BrushGenerator.GenerateTexture(64, h, m_TestSprite.GetTexture());
	}
	TempSelectVertices( m_Model, m_SelectedVertices );	// TODO: Remove when real vertice selection is implemented.
	m_VerticeTranslation.SetSelectedVertices( m_SelectedVertices );
	m_VerticeTranslation.Update( dt );


	// UV
	m_uvTranslation.Update(dt);
	SetWireFrameState(m_VerticeSelection.Update(dt));
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(glm::vec3(0));
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
	m_uvTranslation.Draw(rq);
	m_VerticeSelection.Draw(rq, ro);

	static float brushSize = 64;
	ImGui::SliderFloat("BrushSize", &brushSize, 1, 640);
	m_BrushArea.SetBrushSize(brushSize);
	m_BrushArea.PushStrokes(rq);

	m_BrushGhost.SetSize(glm::vec2(brushSize));
	ImGuiIO io = ImGui::GetIO();
	m_BrushGhost.SetPos(glm::vec2(io.MousePos.x - m_BrushGhost.GetSize().x * 0.5f, io.MousePos.y - m_BrushGhost.GetSize().y * 0.5f));
	rq->Enqueue(m_BrushGhost);

	m_LoadModelButton.Draw(rq);
}

void Game::Shutdown()
{
	m_uvTranslation.Shutdown();
}
void Game::UpdateModelViewWindow(float p_deltaTime)
{
	m_TestArea2.Update();
	float moveSpeed = p_deltaTime * 5.0f;
	float rotationSpeed = p_deltaTime * 1.0f;
	if (m_TestArea2.GetSpaceState())
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

		m_Camera->PitchRelative(ImGui::GetIO().MouseDelta.y * -0.005f);
		m_Camera->RotateAroundNormalizedAxis(glm::vec3(0, 1, 0), ImGui::GetIO().MouseDelta.x * -0.005f);
	}
	else
	{
		//if (m_TestArea2.GetWState())
		//{
		//	m_Camera->PitchRelative(rotationSpeed);
		//}
		//if (m_TestArea2.GetSState())
		//{
		//	m_Camera->PitchRelative(-rotationSpeed);
		//}
		//if (m_TestArea2.GetAState())
		//{
		//	m_Camera->YawRelative(rotationSpeed);
		//}
		//if (m_TestArea2.GetDState())
		//{
		//	m_Camera->YawRelative(-rotationSpeed);
		//}
		//if (m_TestArea2.GetQState())
		//{
		//	m_Camera->YawRelative(-rotationSpeed);
		//	glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
		//	newPos.x -= moveSpeed;
		//	m_Camera->MoveRelative(newPos);
		//}
		//if (m_TestArea2.GetEState())
		//{
		//	m_Camera->YawRelative(rotationSpeed);
		//	glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
		//	newPos.x += moveSpeed;
		//	m_Camera->MoveRelative(newPos);
		//}
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