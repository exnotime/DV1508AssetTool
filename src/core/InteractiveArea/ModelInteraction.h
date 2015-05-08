#pragma once
#include <glm/glm.hpp>
class ModelInteraction
{
public:
	ModelInteraction();
	~ModelInteraction();

	void Initialize(const glm::vec2& size, const glm::vec2& pos);
	void SetSpaceSize(const glm::vec2& size);
	void SetSpacePos(const glm::vec2& pos);
	glm::vec2 GetSpaceSize();
	glm::vec2 GetSpacePos();

	void Update();

	glm::vec2 GetMousePos();

	bool GetLeftMouseClicked();
	bool GetLeftMousePressed();
	bool GetRightMouseClicked();
	bool GetRightMousePressed();
	int GetMouseWheelState();
	bool GetRightMouseDoubleClicked();
	bool GetLeftMouseDoubleClicked();
	bool GetMouseWheelClicked();
	bool GetCtrlButtonPressed();

private:
	void CheckMouseInsideWorkspace();

	glm::vec2 m_SpacePosition;
	glm::vec2 m_SpaceSize;
	glm::vec2 m_ClickPos;
	glm::vec2 m_MousePos;


	bool m_LeftMouseIsPressed;//True if pressed
	bool m_PrevLeftMouseState;//True if pressed
	bool m_RightMouseIsPressed;//True if pressed
	bool m_PrevRightMouseState;//True if pressed

	bool m_rightMouseButtonDoubleClick;
	bool m_leftMouseButtonDoubleClick;

	bool m_mouseInSideWorkspace;

	int m_mouseWheelState;
	bool m_mouseWheelClicked;

	bool m_controlButtonPressed;


};

