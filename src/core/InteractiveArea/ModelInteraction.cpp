#include "ModelInteraction.h"


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
	m_controlButtonPressed = false;
	m_AddKeysForInput = true;
	m_WIsPressed = false;
	m_AIsPressed = false;
	m_SIsPressed = false;
	m_DIsPressed = false;
	m_QIsPressed = false;
	m_EIsPressed = false;
	m_SpaceIsPressed = false;
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
void ModelInteraction::AddKeys(ImGuiIO& p_io)
{
	if (m_AddKeysForInput)
	{
		p_io.AddInputCharacter('w');
		p_io.AddInputCharacter('a');
		p_io.AddInputCharacter('s');
		p_io.AddInputCharacter('d');
		p_io.AddInputCharacter('q');
		p_io.AddInputCharacter('e');
		m_AddKeysForInput = false;
	}
}
void ModelInteraction::Update()
{
	CheckMouseInsideWorkspace();

	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureKeyboard = true;
	if (io.WantCaptureKeyboard)
	{
		AddKeys(io);
		KeyboardUpdate(io);
	}
	MouseUpdate(io);
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
}
void ModelInteraction::KeyboardUpdate(ImGuiIO& p_io)
{
	if (p_io.KeyCtrl)
	{
		m_controlButtonPressed = true;
	}
	else
	{
		m_controlButtonPressed = false;
	}

	//for (int i = 0; i < 512; i++)
	//{
	//	if (p_io.KeysDown[i] == true)
	//	{
	//		int a = 0;
	//	}
	//}
	if (p_io.KeysDown[87])
		m_WIsPressed = true;
	else
		m_WIsPressed = false;
	if (p_io.KeysDown[65])
		m_AIsPressed = true;
	else
		m_AIsPressed = false;
	if (p_io.KeysDown[83])
		m_SIsPressed = true;
	else
		m_SIsPressed = false;
	if (p_io.KeysDown[68])
		m_DIsPressed = true;
	else
		m_DIsPressed = false;
	if (p_io.KeysDown[81])
		m_QIsPressed = true;
	else
		m_QIsPressed = false;
	if (p_io.KeysDown[69])
		m_EIsPressed = true;
	else
		m_EIsPressed = false;
	if (p_io.KeysDown[32])
		m_SpaceIsPressed = true;
	else
		m_SpaceIsPressed = false;


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

bool ModelInteraction::GetCtrlButtonPressed()
{
	return m_controlButtonPressed;
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