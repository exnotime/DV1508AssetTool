#pragma once

#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

class UVTranslation
{
public:
	//UVTranslation();
	//~UVTranslation();

	void Initialize();
	void Update(const float deltaTime);
	void Draw(gfx::RenderQueue* renderQueue);
	
private:

};

