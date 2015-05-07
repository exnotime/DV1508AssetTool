#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

class UVTranslation
{
public:
	//UVTranslation();
	//~UVTranslation();

	void Initialize(gfx::ModelHandle modelHandle);
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);
	void Shutdown();
	
private:
	gfx::Sprite m_dotSprite1;
	gfx::Sprite m_dotSprite2;
	gfx::Sprite m_dotSprite3;
	float* m_uvPos1;
	float* m_uvPos2;
	float* m_uvPos3;

	bool m_uvModeOn;
};

