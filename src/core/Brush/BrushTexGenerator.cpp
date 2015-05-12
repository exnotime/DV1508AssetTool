#include "BrushTexGenerator.h"
#include "../gfx/MaterialBank.h"
BrushTexGenerator::BrushTexGenerator(){

}

BrushTexGenerator::~BrushTexGenerator(){

}

void BrushTexGenerator::Init(){
	m_Program = gfx::g_ShaderBank.LoadShaderProgram("shader/BrushTexGenProgram.glsl");
}

void BrushTexGenerator::GenerateTexture(int size, float hardness, TextureHandle tex){
	gfx::Texture* texture = gfx::g_MaterialBank.GetTexture(tex);
	texture->Resize(size, size);

	gfx::ShaderProgram* prog = gfx::g_ShaderBank.GetProgramFromHandle(m_Program);
	prog->Apply();
	prog->SetUniformFloat("hardness", hardness);
	glBindImageTexture(0, texture->GetHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	GLint x = size / 32;
	glDispatchCompute(size, size, 1);
	glUseProgram(0);
}