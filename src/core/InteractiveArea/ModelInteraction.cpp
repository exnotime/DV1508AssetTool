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

	if (io.MouseDown[0])
	{
		if (m_mouseInSideWorkspace)
		{
			m_LeftMouseIsPressed = true;
		}
	}
	else
	{
		m_LeftMouseIsPressed = false;
	}
	if (io.MouseDown[1])
	{
		if (m_mouseInSideWorkspace)
		{
			m_RightMouseIsPressed = true;
		}
	}
	else
	{
		m_RightMouseIsPressed = false;
	}
	m_MousePos.x = io.MousePos.x;
	m_MousePos.y = io.MousePos.y;
}

glm::vec2 ModelInteraction::GetMousePos()
{
	return m_MousePos;
}
bool ModelInteraction::GetLeftMouseClicked()
{
	//TODO
	return true;
}
bool ModelInteraction::GetLeftMousePressed()
{
	return m_LeftMouseIsPressed;
}
bool ModelInteraction::GetRightMouseClicked()
{
	//TODO
	return true;
}
bool ModelInteraction::GetRightMousePressed()
{
	return m_RightMouseIsPressed;
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