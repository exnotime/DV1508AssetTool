#pragma once
#include "../gfx/Sprite.h"
#include "../InteractiveArea/InteractiveArea.h"
namespace gfx{
	class RenderQueue;
}
class Button{
public:
	Button();
	Button(glm::vec2 pos, glm::vec2 size, const char* filename);
	~Button();
	void Update();
	void Draw(gfx::RenderQueue* rq);
	void SetPos(glm::vec2 pos);
	void SetSize(glm::vec2 size);
	void SetTooltip(const std::string& tooltip);
	void SetSprite(gfx::Sprite spr);
	bool IsClicked();
	bool IsHovering();
private:
	gfx::Sprite m_Sprite;
	InteractiveArea m_IntArea;
	glm::vec2 m_Pos;
	glm::vec2 m_Size;
	std::string m_Tooltip;
	bool m_Clicked;
	bool m_Hovering;
};
