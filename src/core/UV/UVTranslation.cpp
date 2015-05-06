#include "UVTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

//UVTranslation::UVTranslation(){}
//UVTranslation::~UVTranslation(){}

void UVTranslation::Initialize()
{
	int index = 0;
	//ImGui::Begin( "UV" );
	////ImGui::SliderInt("Test", &index, 0, 1);
	//ImGui::End();
}

void UVTranslation::Update(const float deltaTime)
{
}

void UVTranslation::Draw(gfx::RenderQueue* renderQueue)
{

	ImGui::Begin( "UV" );
	////ImGui::SliderInt("Test", &index, 0, 1);
	ImGui::End();

}