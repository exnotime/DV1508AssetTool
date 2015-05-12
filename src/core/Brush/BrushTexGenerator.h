#pragma once
#include "../gfx/ShaderBank.h"
class BrushTexGenerator{
public:
	BrushTexGenerator();
	~BrushTexGenerator();
	void Init();
	void GenerateTexture(int size, float hardness, TextureHandle tex); //tex is the texture that the method will store the generated texture
private:
	gfx::ShaderProgramHandle m_Program = 0;
};