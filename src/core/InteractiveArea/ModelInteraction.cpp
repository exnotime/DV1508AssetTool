#include "ModelInteraction.h"

#include "../gfx/RenderQueue.h"

ModelInteraction::ModelInteraction(){}
ModelInteraction::~ModelInteraction(){}

void ModelInteraction::Initialize(const glm::vec2& p_size, const glm::vec2& p_pos)
{
	SetSpaceSize(p_size);
	SetSpacePos(p_pos);
	m_LeftMouseIsPressed = false;
	m_PrevLeftMouseState = false;
	m_RightMouseIsPressed = false;
	m_PrevRightMouseState = false;
	m_mouseInSideWorkspace = false;
	m_mouseWheelState = 0;
	m_rightMouseButtonDoubleClick = false;
	m_leftMouseButtonDoubleClick = false;
	m_mouseWheelClicked = false;
	m_AddKeysForInput = true;
	m_WIsPressed = false;
	m_AIsPressed = false;
	m_SIsPressed = false;
	m_DIsPressed = false;
	m_QIsPressed = false;
	m_EIsPressed = false;
	m_SpaceIsPressed = false;
	m_switchInputTypeState = false;
	m_prevSwitchInputTypeState = false;
	m_LeftArrowIsPressed = false;
	m_RightArrowIsPressed = false;
	m_UpArrowIsPressed = false;
	m_DownArrowIsPressed = false;
	m_CMC = CMC_FirstPerson;
	m_renderModeSelectionButtons = false;

	m_buttonXPos = 300;
	m_buttonYPos = 1000;
	glm::vec2 mouseSpriteSize = glm::vec2(25, 25);

	m_CameraModeSelection = Button(glm::vec2(m_buttonXPos, m_buttonYPos), glm::vec2(50, 50), "asset/Icons/S_Camera_Mode.png");
	m_CameraModeSelection.SetTooltip("Select Camera Movement Mode");
	m_DrawnSprite.SetTexture("asset/Icons/CameraModes/Mode_FPS.png");
	m_DrawnSprite.SetSize(mouseSpriteSize);

	m_FirstPersonModeButton = Button(glm::vec2(m_buttonXPos, m_buttonYPos - 50), glm::vec2(50, 50), "asset/Icons/S_FPS.png");
	m_FirstPersonModeButton.SetTooltip(" First Person Camera \n Move camera using WASD \n Rotate camera using mouse");
	m_FirstPersonModeSprite.SetTexture("asset/Icons/CameraModes/Mode_FPS.png");
	m_FirstPersonModeSprite.SetSize(mouseSpriteSize);

	m_LaptopModeButton = Button(glm::vec2(m_buttonXPos - 50, m_buttonYPos - 50), glm::vec2(50, 50), "asset/Icons/S_Keyboard.png");
	m_LaptopModeButton.SetTooltip(" Laptop mode \n Move camera using WASD \n Rotate Camera using arrow keys");
	m_LaptopModeSprite.SetTexture("asset/Icons/CameraModes/Mode_Keys.png");
	m_LaptopModeSprite.SetSize(mouseSpriteSize);

	m_MouseModeButton = Button(glm::vec2(m_buttonXPos + 50, m_buttonYPos - 50), glm::vec2(50, 50), "asset/Icons/S_Grabber.png");
	m_MouseModeButton.SetTooltip(" Mouse mode \n Use left mouse button to orbit the model \n Use scroll to get closer or further away from the model");
	m_MouseModeSprite.SetTexture("asset/Icons/CameraModes/Mode_Drag.png");
	m_MouseModeSprite.SetSize(mouseSpriteSize);
}
void ModelInteraction::SetSpaceSize(const glm::vec2& p_size)
{
	m_SpaceSize = p_size;
}
void ModelInteraction::SetSpacePos(const glm::vec2& p_pos)
{
	m_SpacePosition = p_pos;
}
glm::vec2 ModelInteraction::GetSpaceSize()
{
	return m_SpaceSize;
}
glm::vec2 ModelInteraction::GetSpacePos()
{
	return m_SpacePosition;
}

void ModelInteraction::Update()
{

	CheckMouseInsideWorkspace();

	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureKeyboard = true;
	if (io.WantCaptureKeyboard)
	{
		KeyboardUpdate(io);
	}
	MouseUpdate(io);

	m_CameraModeSelection.Update();
	if (m_CameraModeSelection.IsClicked())
	{
		if (m_renderModeSelectionButtons)
		{
			m_renderModeSelectionButtons = false;
		}
		else
		{
			m_renderModeSelectionButtons = true;
		}
	}

	if (m_renderModeSelectionButtons)
	{
		m_LaptopModeButton.Update();
		m_MouseModeButton.Update();
		m_FirstPersonModeButton.Update();
	}
	
}
void ModelInteraction::MouseUpdate(ImGuiIO& p_io)
{
	m_PrevLeftMouseState = m_LeftMouseIsPressed;
	m_PrevRightMouseState = m_RightMouseIsPressed;
	if (p_io.MouseDown[0])
	{
		m_LeftMouseIsPressed = true;
	}
	else
	{
		m_LeftMouseIsPressed = false;
	}
	if (p_io.MouseDown[1])
	{
		m_RightMouseIsPressed = true;
	}
	else
	{
		m_RightMouseIsPressed = false;
	}
	if (p_io.MouseClicked[2])
	{
		m_mouseWheelClicked = true;
	}
	else
	{
		m_mouseWheelClicked = false;
	}
	if (p_io.MouseDoubleClicked[0])
	{
		m_leftMouseButtonDoubleClick = true;
	}
	else
	{
		m_leftMouseButtonDoubleClick = false;
	}
	if (p_io.MouseDoubleClicked[1])
	{
		m_rightMouseButtonDoubleClick = true;
	}
	else
	{
		m_rightMouseButtonDoubleClick = false;
	}

	m_mouseWheelState = (int)p_io.MouseWheel;
	m_MousePos.x = p_io.MousePos.x;
	m_MousePos.y = p_io.MousePos.y;

	m_DrawnSprite.SetPos(glm::vec2(m_MousePos.x + 15, m_MousePos.y + 15));
}
void ModelInteraction::KeyboardUpdate(ImGuiIO& p_io)
{
	//Användes för att hitta vilken index en tangent har
	for (int i = 0; i < 512; i++)
	{
		if (p_io.KeysDown[i] == true)
		{
			int a = 0;
		}
	}
	if (p_io.KeysDown[87])//W 
		m_WIsPressed = true;
	else
		m_WIsPressed = false;
	if (p_io.KeysDown[65])//A
		m_AIsPressed = true;
	else
		m_AIsPressed = false;
	if (p_io.KeysDown[83])//S
		m_SIsPressed = true;
	else
		m_SIsPressed = false;
	if (p_io.KeysDown[68])//D
		m_DIsPressed = true;
	else
		m_DIsPressed = false;
	if (p_io.KeysDown[81])//Q
		m_QIsPressed = true;
	else
		m_QIsPressed = false;
	if (p_io.KeysDown[69])//E
		m_EIsPressed = true;
	else
		m_EIsPressed = false;
	if (p_io.KeysDown[32])//Space
		m_SpaceIsPressed = true;
	else
		m_SpaceIsPressed = false;
	if (p_io.KeysDown[263])//Left arrow
		m_LeftArrowIsPressed = true;
	else
		m_LeftArrowIsPressed = false;
	if (p_io.KeysDown[265])//Up arrow
		m_UpArrowIsPressed = true;
	else
		m_UpArrowIsPressed = false;
	if (p_io.KeysDown[264])//Down arrow
		m_DownArrowIsPressed = true;
	else
		m_DownArrowIsPressed = false;
	if (p_io.KeysDown[262])//Right arrow
		m_RightArrowIsPressed = true;
	else
		m_RightArrowIsPressed = false;

	UpdateSwitchInputType(p_io);
}
void ModelInteraction::UpdateSwitchInputType(ImGuiIO& p_io)
{
	if (m_FirstPersonModeButton.IsClicked() && !m_CameraModeSelection.IsClicked())
	{
		//m_renderModeSelectionButtons = false;
		m_CMC = CMC_FirstPerson;
		m_CameraModeSelection = Button(glm::vec2(m_buttonXPos, m_buttonYPos), glm::vec2(50, 50), "asset/Icons/S_FPS.png");
		m_DrawnSprite = m_FirstPersonModeSprite;
	}
	if (m_LaptopModeButton.IsClicked() && !m_CameraModeSelection.IsClicked())
	{
		//m_renderModeSelectionButtons = false;
		m_CMC = CMC_LaptopMode;
		m_CameraModeSelection = Button(glm::vec2(m_buttonXPos, m_buttonYPos), glm::vec2(50, 50), "asset/Icons/S_Keyboard.png");
		m_DrawnSprite = m_LaptopModeSprite;
	}
	if (m_MouseModeButton.IsClicked() && !m_CameraModeSelection.IsClicked())
	{
		//m_renderModeSelectionButtons = false;
		m_CMC = CMC_MouseOnly;
		m_CameraModeSelection = Button(glm::vec2(m_buttonXPos, m_buttonYPos), glm::vec2(50, 50), "asset/Icons/S_Grabber.png");
		m_DrawnSprite = m_MouseModeSprite;
	}
}

CameraMovementControls ModelInteraction::GetCMC()
{
	return m_CMC;
}
glm::vec2 ModelInteraction::GetMousePos()
{
	return m_MousePos;
}
bool ModelInteraction::GetLeftMouseClicked()
{
	if (!m_LeftMouseIsPressed && m_PrevLeftMouseState)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}
bool ModelInteraction::GetLeftMousePressed()
{
	if (m_LeftMouseIsPressed)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
		return false;
	}
	return false;
}
bool ModelInteraction::GetRightMouseClicked()
{
	if (!m_RightMouseIsPressed && m_PrevRightMouseState)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}
bool ModelInteraction::GetRightMousePressed()
{
	if (m_RightMouseIsPressed)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
		return false;
	}
	return false;
}

void ModelInteraction::CheckMouseInsideWorkspace()
{
	if ((m_MousePos.x < (m_SpacePosition.x + m_SpaceSize.x)) && (m_MousePos.x > m_SpacePosition.x))
	{
		if ((m_MousePos.y < (m_SpacePosition.y + m_SpaceSize.y)) && (m_MousePos.y > m_SpacePosition.y))
		{
			m_mouseInSideWorkspace = true;
			return;
		}
	}
	m_mouseInSideWorkspace = false;
}

int ModelInteraction::GetMouseWheelState()
{
	if (m_mouseWheelState)
	{
		if (m_mouseInSideWorkspace)
		{
			return m_mouseWheelState;
		}
	}
	return 0;
}
bool ModelInteraction::GetRightMouseDoubleClicked()
{
	if (m_rightMouseButtonDoubleClick)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
	}
	return false;
}
bool ModelInteraction::GetLeftMouseDoubleClicked()
{
	if (m_leftMouseButtonDoubleClick)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
	}
	return false;
}
bool ModelInteraction::GetMouseWheelClicked()
{
	if (m_mouseWheelClicked)
	{
		if (m_mouseInSideWorkspace)
		{
			return true;
		}
	}
	return false;
}

bool ModelInteraction::GetWState()
{
	return m_WIsPressed;
}
bool ModelInteraction::GetAState()
{
	return m_AIsPressed;
}
bool ModelInteraction::GetSState()
{
	return m_SIsPressed;
}
bool ModelInteraction::GetDState()
{
	return m_DIsPressed;
}
bool ModelInteraction::GetQState()
{
	return m_QIsPressed;
}
bool ModelInteraction::GetEState()
{
	return m_EIsPressed;
}
bool ModelInteraction::GetSpaceState()
{
	return m_SpaceIsPressed;
}
bool ModelInteraction::GetLeftArrowState()
{
	return m_LeftArrowIsPressed;
}
bool ModelInteraction::GetUpArrowState()
{
	return m_UpArrowIsPressed;
}
bool ModelInteraction::GetDownArrowState()
{
	return m_DownArrowIsPressed;
}
bool ModelInteraction::GetRightArrowState()
{
	return m_RightArrowIsPressed;
}
void ModelInteraction::RenderButtons(gfx::RenderQueue* rq)
{
	m_CameraModeSelection.Draw(rq);
	if (m_renderModeSelectionButtons)
	{
		m_FirstPersonModeButton.Draw(rq);
		m_LaptopModeButton.Draw(rq);
		m_MouseModeButton.Draw(rq);
	}
	if (m_mouseInSideWorkspace)
	{
		rq->Enqueue(m_DrawnSprite);
	}
}