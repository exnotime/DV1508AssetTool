#pragma once
#include <glm/glm.hpp>
class InteractiveArea{
public:
	InteractiveArea();
	~InteractiveArea();
	void SetSize(const glm::vec2& size);
	void SetPos(const glm::vec2& pos);
	glm::vec2 GetSize();
	glm::vec2 GetPos();
	bool IsClicked(glm::vec2& outPos); //returns if the area is clicked, outPos contains the position in 0 -> 1 space inside the area
	void Update();
private:
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	glm::vec2 m_ClickPos;
	bool m_ClickedThisFrame;
	bool m_ClickedLastFrame;
};