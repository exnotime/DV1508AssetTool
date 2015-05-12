#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../InteractiveArea/InteractiveArea.h"
#include "../gfx/RenderQueue.h"
#define BRUSH_DELTA 0.1f
class BrushArea{
public:
	BrushArea();
	~BrushArea();
	void Update();
	float GetBrushSize();
	void SetBrushSize(float size);
	InteractiveArea& GetArea();
	void SetArea(const InteractiveArea& area);
	TextureHandle GetTexture();
	void SetTexture(TextureHandle handle);
	void PushStrokes(gfx::RenderQueue* rq);
private:
	float m_BrushSize;
	std::vector<gfx::BrushObject> m_BrushObjects;
	InteractiveArea m_Area;
	bool m_IsActive;
	glm::vec2 m_MousePos;
	TextureHandle m_BrushTexture;
};