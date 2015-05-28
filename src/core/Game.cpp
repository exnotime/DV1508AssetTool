#include "Game.h"
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/MaterialBank.h"
#include "../gfx/Material.h"
#include <nfd/nfd.h>
#include "../gfx/GraphicsEngine.h"
Game::Game( )
{
}


Game::~Game( )
{

}

void Game::Initialize(int width, int height){
	m_Model = gfx::g_ModelBank.LoadModel("asset/Bomb/model.obj");
	m_VerticeTranslation.Initialize();
	m_uvTranslation.Initialize();
	m_VerticeSelection.Initialize();
	m_TestSprite.SetTexture("asset/brush.png");
	m_BrushGhost.SetTexture("asset/BrushGhost.png");
	m_TestArea.SetPos(glm::vec2(width / 2, BUTTON_SIZE));
	m_TestArea.SetSize(glm::vec2(width / 2 , height - BUTTON_SIZE * 2));

	m_BrushArea.SetArea(m_TestArea);
	m_BrushArea.SetBrushSize(64);
	m_BrushArea.SetTexture(m_TestSprite.GetTexture());

	m_BrushGenerator.Init();
	m_BrushGenerator.GenerateTexture(64, 0.5f, m_TestSprite.GetTexture());

	m_LoadModelButton = Button(glm::vec2(width * 0.5f - BUTTON_SIZE * 0.5f, 25.0f + BUTTON_SIZE), glm::vec2(BUTTON_SIZE), "asset/Icons/S_Load_Model.png");
	m_LoadModelButton.SetTooltip("Load Model");

	m_UndoButton = Button(glm::vec2(width * 0.5f - BUTTON_SIZE * 0.5f, 25.0f + 2 * BUTTON_SIZE), glm::vec2(BUTTON_SIZE), "asset/Icons/S_Undo.png");
	m_UndoButton.SetTooltip("Undo (Ctrl+Z)");

	m_RedoButton = Button(glm::vec2(width * 0.5f - BUTTON_SIZE * 0.5f, 25.0f + 3 * BUTTON_SIZE), glm::vec2(BUTTON_SIZE), "asset/Icons/S_Redo.png");
	m_RedoButton.SetTooltip("Redo (Ctrl+Y)");

	m_CloseProgramButton = Button(glm::vec2(width - BUTTON_SIZE, 0), glm::vec2(BUTTON_SIZE), "asset/Icons/P_Exit.png");
	m_CloseProgramButton.SetTooltip("Close program");

	m_ColorPickerButton = Button(glm::vec2(width - BUTTON_SIZE * 2.0f - 30.0f, height - BUTTON_SIZE - 25.0f), glm::vec2(BUTTON_SIZE), "asset/Icons/T_Palette.png");
	m_ColorPickerButton.SetTooltip("Color Picker");

	m_ColorPickerButtonOverlay.SetPos(glm::vec2(width - BUTTON_SIZE - 25.0f, height - BUTTON_SIZE - 25.0f));
	m_ColorPickerButtonOverlay.SetTexture("asset/Icons/T_Chosen_Color.png");
	m_ColorPickerButtonOverlay.SetSize(glm::vec2(BUTTON_SIZE));

	m_colorPicker.Init(glm::vec2(width - 312.0f - 15.0f, height - 322.0f - 50.0f));

	m_Background.SetTexture("asset/Backgrounds/Background.png");
	m_Background.SetPos(glm::vec2(0.0f, BUTTON_SIZE));
	m_Background.SetSize(glm::vec2(width, height - BUTTON_SIZE));

	m_RelationsBackground.SetTexture("asset/Backgrounds/Relations_Background.png");
	glm::vec2 size = glm::vec2(750, 500);  
	m_RelationsBackground.SetPos(glm::vec2(width * 0.5f - size.x * 0.5f, height * 0.5f - size.y * 0.5f));
	m_RelationsBackground.SetSize(size);

	m_FakeRelations.SetTexture("asset/Backgrounds/Fake_Relations.png");
	m_FakeRelations.SetPos(glm::vec2(width * 0.5f - size.x * 0.5f, height * 0.5f - size.y * 0.5f));
	m_FakeRelations.SetSize(size);

	m_RelationsToggled = false;
	m_RelationsButton = Button(glm::vec2(width * 0.5f - BUTTON_SIZE * 0.5f, height - BUTTON_SIZE - 25.0f), glm::vec2(BUTTON_SIZE), "asset/Icons/S_Relations.png");
	m_RelationsButton.SetTooltip("Open the relations manager, \n here you can edit relations \n between models and textures.");

	m_uvButton = Button(glm::vec2(width * 0.5f - BUTTON_SIZE * 0.5f, height - BUTTON_SIZE * 2 - 25.0f), glm::vec2(BUTTON_SIZE), "asset/Icons/M_UV_Picker_Tool.png");
	m_uvButton.SetTooltip("UV Picker:\nTranslate the UV coordinates");

	///////////////////////////////////////////////////////////////////////////////
	m_TestArea2.Initialize(glm::vec2(width / 2, height - BUTTON_SIZE), glm::vec2(0, BUTTON_SIZE));
	m_AutomaticRotate = false;
	m_AutomaticRotateLeft = false;
	m_MousePos = glm::vec2(0, 0);
	m_PrevMousePos = glm::vec2(0, 0);
	m_Camera = gfx::g_GFXEngine.GetCamera();
	m_Camera->MoveWorld(glm::vec3(0, -8, -15));
	m_StartPos = m_Camera->GetPosition();
	m_StartOrientation = m_Camera->GetOrientation();
	///////////////////////////////////////////////////////////////////////////////
}

void Game::Update(float dt){
	m_BrushBlock = false;
	///////////////////////////////////////////////////////////////////////////////
	UpdateModelViewWindow(dt);	
	///////////////////////////////////////////////////////////////////////////////
	m_BrushArea.Update();
	m_LoadModelButton.Update();
	m_UndoButton.Update();
	m_RedoButton.Update();
	//check if we should close
	m_CloseProgramButton.Update();
	if (m_CloseProgramButton.IsClicked()) glfwSetWindowShouldClose(gfx::g_GFXEngine.GetWindow(), GL_TRUE);
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

			m_uvTranslation.ResetList();
		}
	}
	m_ColorPickerButton.Update();
	if (m_ColorPickerButton.IsClicked()){
		m_colorPicker.TogglePicker();
	}
	static float h = 0;
	static float oldh = 0;
	ImGui::SliderFloat("Hardness", &h, 0, 1);
	if (h != oldh){
		m_BrushGenerator.GenerateTexture(64, h, m_TestSprite.GetTexture());
	}
	oldh = h;
	TempSelectVertices( m_Model, m_SelectedVertices );	// TODO: Remove when real vertice selection is implemented.
	m_VerticeTranslation.SetSelectedVertices( m_SelectedVertices );
	m_VerticeTranslation.Update( dt );

	// UV
	m_uvTranslation.Update(dt);
	m_uvButton.Update();
	if (m_uvButton.IsClicked())
	{
		m_uvTranslation.Toggle();
	}

	SetWireFrameState(m_VerticeSelection.Update(dt));

	m_colorPicker.Update();

	// Relations manager.
	m_RelationsButton.Update();
	if (m_RelationsButton.IsClicked())
	{
		if (m_RelationsToggled)
		{
			m_RelationsToggled = false;
		}

		else
		{
			m_RelationsToggled = true;
		}
	}

	if (m_uvTranslation.IsActive())
	{
		m_BrushBlock = true;
	}
	else
	{
		m_BrushBlock = m_colorPicker.IsActive();
	}
}

void Game::Render( gfx::RenderQueue* rq )
{
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(glm::vec3(0));
	SetWireFrameModel(ro);
	rq->Enqueue(ro);

	// Render background.
	rq->Enqueue(m_Background);

	// Set texture.
	gfx::Model model = gfx::g_ModelBank.FetchModel(m_Model);

	static int meshIndex = 0;
	ImGui::SliderInt("Mesh Texture", &meshIndex, 0, (int)(model.Meshes.size() - 1));
	//fixing error with imgui when min and max is the same
	if (meshIndex < 0)
		meshIndex = 0;
	gfx::Material* mat = gfx::g_MaterialBank.GetMaterial(model.MaterialOffset + model.Meshes[meshIndex].Material);
	static int actTexture = 0;
	ImGui::SliderInt("TextureType", &actTexture, 0, 2);
	switch (actTexture){
		case 0:
			rq->SetTargetTexture(mat->GetAlbedoTexture());
		break;
		case 1:
			rq->SetTargetTexture(mat->GetRoughnessTexture());
		break;
		case 2:
			rq->SetTargetTexture(mat->GetMetalTexture());
		break;
	};

	m_VerticeTranslation.Draw( rq );
	m_VerticeSelection.Draw(rq, ro);

	m_uvTranslation.Draw(rq);
	m_uvButton.Draw(rq);

	static float brushSize = 64;
	ImGui::SliderFloat("BrushSize", &brushSize, 1, 640);
	m_BrushArea.SetBrushSize(brushSize);
	if (!m_BrushBlock)
		m_BrushArea.PushStrokes(rq);
	else
		m_BrushArea.ClearStrokes();

	m_BrushGhost.SetSize(glm::vec2(brushSize));
	ImGuiIO io = ImGui::GetIO();
	m_BrushGhost.SetPos(glm::vec2(io.MousePos.x - m_BrushGhost.GetSize().x * 0.5f, io.MousePos.y - m_BrushGhost.GetSize().y * 0.5f));
	rq->Enqueue(m_BrushGhost);

	m_LoadModelButton.Draw(rq);
	m_UndoButton.Draw(rq);
	m_RedoButton.Draw(rq);
	m_ColorPickerButton.Draw(rq);
	m_ColorPickerButtonOverlay.SetColor(ColorPicker::m_color);
	rq->Enqueue(m_ColorPickerButtonOverlay);

	m_colorPicker.Draw(rq);
	m_TestArea2.RenderButtons(rq);
	m_CloseProgramButton.Draw(rq);

	m_RelationsButton.Draw(rq);
	if (m_RelationsToggled)
	{
		rq->Enqueue(m_RelationsBackground);
		rq->Enqueue(m_FakeRelations);
	}
	
}

void Game::Shutdown()
{
	m_uvTranslation.Shutdown();
}
void Game::UpdateModelViewWindow(float p_deltaTime)
{
	m_TestArea2.Update();

	if (m_TestArea2.GetCMC() == CMC_FirstPerson)
	{
		UpdateFirstPersonCamera(p_deltaTime);
	}
	else if (m_TestArea2.GetCMC() == CMC_LaptopMode)
	{
		UpdateCameraLaptopMode(p_deltaTime);
	}
	else if (m_TestArea2.GetCMC() == CMC_MouseOnly)
	{
		UpdateMouseInput(p_deltaTime);
	}
	if (m_TestArea2.GetLeftMouseDoubleClicked())
	{
		ResetCamera();
	}
}
void Game::UpdateFirstPersonCamera(float p_deltaTime)
{
	float moveSpeed = p_deltaTime * 3.0f;
	float rotationSpeed = p_deltaTime * 0.75f;
	if (m_TestArea2.GetSpaceState())
	{
		glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
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
		float distanceToModel = m_Camera->GetPosition().z;
		if (distanceToModel < 0)
			distanceToModel *= -1;
		float rotationVariable = distanceToModel / m_StartPos.z;
		if (rotationVariable < 1.0)
			rotationVariable = 1.0f;
		m_Camera->PitchRelative(ImGui::GetIO().MouseDelta.y * -0.005f * rotationVariable);
		m_Camera->RotateAroundNormalizedAxis(glm::vec3(0, 1, 0), ImGui::GetIO().MouseDelta.x * -0.005f * rotationVariable);
	}
}
void Game::UpdateMouseInput(float p_deltaTime)
{
	float distanceToModel = m_Camera->GetPosition().z;
	if (distanceToModel < 0)
		distanceToModel *= -1;
	float rotationVariable = m_StartPos.z/distanceToModel;
	if (rotationVariable < 1.0)
		rotationVariable = 1.0f;

	float moveSpeed = p_deltaTime * 3.0f * rotationVariable;
	float rotationSpeed = p_deltaTime * 4.0f*rotationVariable;
	m_PrevMousePos = m_MousePos;
	m_MousePos = m_TestArea2.GetMousePos();
	//Rotate the model around Y
	if (m_TestArea2.GetSpaceState())
	{
		if (m_TestArea2.GetLeftMousePressed())
		{
			glm::vec3 newPos = m_Camera->GetPosition();
			glm::vec3 currentPos = m_Camera->GetPosition();

			float x = m_PrevMousePos.x - m_MousePos.x;
			float y = m_PrevMousePos.y - m_MousePos.y;
			float moveCameraSpeed = moveSpeed * 8.0f;
			float rotateCameraSpeed = rotationSpeed * 1.2f;
			if (x != 0.0f)
			{
				if (x < 0.0f)
				{
					m_Camera->YawRelative(-rotateCameraSpeed);
					newPos.x -= (moveCameraSpeed);
				}
				else
				{
					m_Camera->YawRelative(rotateCameraSpeed);
					newPos.x += (moveCameraSpeed);
				}
			}
			if (y != 0.0f)
			{
				if (y < 0.0f)
				{
					m_Camera->PitchRelative(-rotateCameraSpeed);
					newPos.y += (moveCameraSpeed);
				}
				else
				{
					m_Camera->PitchRelative(rotateCameraSpeed);
					newPos.y -= (moveCameraSpeed);
				}
			}

			glm::vec3 finalPosition = glm::vec3(newPos.x - currentPos.x, newPos.y - currentPos.y, newPos.z - currentPos.z);
			m_Camera->MoveRelative(finalPosition);


		}
		//Move the model around in the viewspace
		if (m_TestArea2.GetRightMousePressed())
		{
			float x = 0, y = 0;
			x = m_PrevMousePos.x - m_MousePos.x;
			y = m_PrevMousePos.y - m_MousePos.y;
			glm::vec3 newPos = m_Camera->GetPosition();
			newPos.x += x * p_deltaTime;
			newPos.y -= y * p_deltaTime;

			glm::vec3 currentPos = m_Camera->GetPosition();
			glm::vec3 finalPosition = glm::vec3(newPos.x - currentPos.x, newPos.y - currentPos.y, newPos.z - currentPos.z);
			m_Camera->MoveRelative(finalPosition);
		}
		//Manipulate scale by scrolling the mouse wheel
		m_Camera->MoveRelative(glm::vec3(0, 0, -m_TestArea2.GetMouseWheelState() * p_deltaTime * 100));

		if (m_TestArea2.GetLeftMouseDoubleClicked() && m_StartPos == m_Camera->GetPosition() && !m_AutomaticRotate)
		{
			m_AutomaticRotate = true;
		}
		else if (m_TestArea2.GetLeftMouseDoubleClicked() && m_AutomaticRotate)
		{
			m_AutomaticRotate = false;
		}
		else if (m_TestArea2.GetLeftMouseDoubleClicked())
		{
			ResetCamera();
		}
	}
	//Rotates automatically
	if (m_AutomaticRotate)
	{
		glm::vec3 newPos = m_Camera->GetPosition();
		glm::vec3 currentPos = m_Camera->GetPosition();

		if (m_AutomaticRotateLeft)
		{
			m_Camera->YawRelative(-rotationSpeed * 0.3f);
			newPos.x -= (moveSpeed * 2.0f);
			glm::vec3 finalPosition = glm::vec3(newPos.x - currentPos.x, newPos.y - currentPos.y, newPos.z - currentPos.z);
			m_Camera->MoveRelative(finalPosition);
		}
		else
		{
			m_Camera->YawRelative(rotationSpeed * 0.3f);
			newPos.x += (moveSpeed * 2.0f);
			glm::vec3 finalPosition = glm::vec3(newPos.x - currentPos.x, newPos.y - currentPos.y, newPos.z - currentPos.z);
			m_Camera->MoveRelative(finalPosition);
		}
	}
}
void Game::UpdateCameraLaptopMode(float p_deltaTime)
{
	float moveSpeed = p_deltaTime * 3.0f;
	float distanceToModel = m_Camera->GetPosition().z;
	if (distanceToModel < 0)
		distanceToModel *= -1;
	float rotationVariable = distanceToModel / m_StartPos.z;
	if (rotationVariable < 1.0)
		rotationVariable = 1.0f;
	float rotationSpeed = p_deltaTime * 0.75f * rotationVariable;
	if (!m_TestArea2.GetSpaceState())
	{
		glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f);
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

		if (m_TestArea2.GetUpArrowState())
		{
			m_Camera->PitchRelative(rotationSpeed);
		}
		if (m_TestArea2.GetDownArrowState())
		{
			m_Camera->PitchRelative(-rotationSpeed);
		}
		if (m_TestArea2.GetLeftArrowState())
		{
			m_Camera->YawRelative(rotationSpeed);
		}
		if (m_TestArea2.GetRightArrowState())
		{
			m_Camera->YawRelative(-rotationSpeed);
		}
	}
	}
void Game::ResetCamera()
{
	m_Camera->SetPosition(m_StartPos);
	m_Camera->SetOrientation(m_StartOrientation);
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