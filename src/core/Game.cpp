#include "Game.h"
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/MaterialBank.h"
#include "../gfx/Material.h"
#include "../gfx/GraphicsEngine.h"
#include "../gfx/Camera.h"
Game::Game( )
{
}


Game::~Game( )
{

}

void Game::Initialize(){
	m_TargetTex = gfx::g_MaterialBank.LoadTexture("asset/flcl.jpg", gfx::TEXTURE_COLOR);
	m_Model = gfx::g_ModelBank.LoadModel("asset/LucinaResource/Lucina_posed.obj");
	m_Pos = glm::vec3(0.0f);
	m_Scale = 1.0f;
	m_RotateY = 0.0f;
	m_VerticeTranslation.Initialize();
	m_TestSprite.SetTexture("asset/brush.png");
	m_TargetTex = gfx::g_MaterialBank.LoadTexture("asset/flcl.jpg",gfx::TEXTURE_COLOR);

	m_TestArea.SetPos(glm::vec2(640, 0));
	m_TestArea.SetSize(glm::vec2(640, 720));
	
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
}

void Game::Render( gfx::RenderQueue* rq ){
	gfx::RenderObject ro;
	ro.Model = m_Model;
	ro.world = glm::translate(m_Pos) * glm::scale(glm::vec3(m_Scale)) * glm::rotate(m_RotateY, glm::vec3(0, 1, 0));
	rq->Enqueue(ro);

	//Set texture
	gfx::Model lucina = gfx::g_ModelBank.FetchModel(m_Model);
	static int meshIndex = 0;
	ImGui::SliderInt("Mesh Texture", &meshIndex, 0, lucina.Meshes.size() - 1);
	gfx::Material* lucMat = gfx::g_MaterialBank.GetMaterial(lucina.Meshes[meshIndex].Material);
	rq->SetTargetTexture(lucMat->GetAlbedoTexture());

	m_VerticeTranslation.Draw( rq );

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
void Game::UpdateModelViewWindow(float p_deltaTime)
{
	//m_Camera->MoveRelative(glm::vec3(0, 0, p_deltaTime));//move camera backward 
	m_PrevMousePos = m_MousePos;
	m_TestArea2.Update();
	m_MousePos = m_TestArea2.GetMousePos();
	//Rotate the model around Y
	if (m_TestArea2.GetCtrlButtonPressed())
	{
		if (m_TestArea2.GetLeftMousePressed())
		{//TODO: ändra orienteringen av kameran... dafuq?
			glm::vec3 newPos = m_Camera->GetPosition();

			float x = m_PrevMousePos.x - m_MousePos.x;
			float y = m_PrevMousePos.y - m_MousePos.y;
			if (x < 0)
			{
				m_AutomaticRotateLeft = false;
			}
			else
			{
				m_AutomaticRotateLeft = true;
			}
			//X-led skall ändra x och z pos
			//Y-led skall ändra y och z pos
			if (x != 0.0f)
			{
				float angle = atan((x* p_deltaTime) / newPos.z);
				float sinAngle = sin(angle);
				float cosAngle = cos(angle);
				float xPos = newPos.x;
				float zPos = newPos.z;
				newPos.x = xPos * cosAngle - zPos * sinAngle;
				newPos.z = zPos * cosAngle + xPos * sinAngle;
				
			}
			if (y != 0.0f)
			{
				float angle = atan((y* p_deltaTime) / newPos.z);
				float sinAngle = sin(angle);
				float cosAngle = cos(angle);
				float yPos = newPos.y;
				float zPos = newPos.z;
				newPos.y = yPos * cosAngle - zPos * sinAngle;
				newPos.z = zPos * cosAngle + yPos * sinAngle;
			}
			m_Camera->SetPosition(newPos);
			//const glm::quat temp = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
			//m_Camera->SetOrientation(temp);
			//m_Camera->CalculateViewProjection();
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
			m_Camera->SetPosition(newPos);
		}
		//Manipulate scale by scrolling the mouse wheel
		m_Camera->MoveRelative(glm::vec3(0, 0, -m_TestArea2.GetMouseWheelState() * p_deltaTime * 20));

		//Reset position, rotation and scale
		if (m_TestArea2.GetMouseWheelClicked())
		{
			glm::vec3 temp = m_Camera->GetPosition();
			temp.z = m_StartScale;
			m_Camera->SetPosition(temp);
		}

		if (m_TestArea2.GetRightMouseDoubleClicked())
		{
			//m_Camera->SetPosition(m_StartPos);
			m_Camera->MoveRelative(glm::vec3(1.0f,0.0f,0.0f));
		}

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
			m_Camera->SetPosition(m_StartPos);
		}
	}
	//Rotates automatically
	if (m_AutomaticRotate)
	{//TODO: Set the camera orientation
		glm::vec3 newPos = m_Camera->GetPosition();
		float xDivZ;
		if (m_AutomaticRotateLeft)
			xDivZ = (p_deltaTime * 5) / newPos.z;
		else
			xDivZ = (-p_deltaTime * 5) / newPos.z;

		float angle = atan(xDivZ);
		float sinAngle = sin(angle);
		float cosAngle = cos(angle);
		float xPos = newPos.x;
		float yPos = newPos.y;
		float zPos = newPos.z;
		newPos.x = xPos * cosAngle - zPos * sinAngle;
		newPos.z = zPos * cosAngle + xPos * sinAngle;


		m_Camera->SetPosition(newPos);
		//m_Camera->SetOrientation(glm::quat(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f));
		//m_Camera->SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		const glm::quat temp = glm::quat(m_Pos.x, m_Pos.y, m_Pos.z, 0.0f);
		m_Camera->SetOrientation(temp);
	}
	m_Camera->CalculateViewProjection();
}