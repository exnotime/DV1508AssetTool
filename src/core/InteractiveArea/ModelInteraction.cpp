#include "ModelInteraction.h"
#include <imgui/imgui.h>


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
	m_PrevLeftMouseState = m_LeftMouseIsPressed;
	m_PrevRightMouseState = m_RightMouseIsPressed;

	ImGuiIO& io = ImGui::GetIO();

	if (io.KeyCtrl)
	{
		m_controlButtonPressed = true;
	}
	else
	{
		m_controlButtonPressed = false;
	}

	if (io.MouseDown[0])
	{
		m_LeftMouseIsPressed = true;
	}
	else
	{
		m_LeftMouseIsPressed = false;
	}
	if (io.MouseDown[1])
	{
		m_RightMouseIsPressed = true;
	}
	else
	{
		m_RightMouseIsPressed = false;
	}
	if (io.MouseClicked[2])
	{
		m_mouseWheelClicked = true;
	}
	else
	{
		m_mouseWheelClicked = false;
	}
	if (io.MouseDoubleClicked[0])
	{
		m_leftMouseButtonDoubleClick = true;
	}
	else
	{
		m_leftMouseButtonDoubleClick = false;
	}
	if (io.MouseDoubleClicked[1])
	{
		m_rightMouseButtonDoubleClick = true;
	}
	else
	{
		m_rightMouseButtonDoubleClick = false;
	}

	m_mouseWheelState = (int)io.MouseWheel;
	m_MousePos.x = io.MousePos.x;
	m_MousePos.y = io.MousePos.y;
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

		// TODO: är detta vad henrik vill? :3 Saknade return path här.
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

		// TODO: är detta vad henrik vill? :3 Saknade return path här.
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

		// TODO: är detta vad henrik vill? :3 Saknade return path här.
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

		// TODO: är detta vad henrik vill? :3 Saknade return path här.
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