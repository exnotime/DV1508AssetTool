#include "VerticeSelection.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/GraphicsEngine.h"
#include "../gfx/Camera.h"

#define EPSILON 1.0e-14f

struct Ray {
	glm::vec3	Position;
	glm::vec3	Direction;
};

VerticeSelection::VerticeSelection(){

}

VerticeSelection::~VerticeSelection(){

}

void VerticeSelection::Initialize(){
	m_Checked = false;


}

bool VerticeSelection::Update(const float deltaTime){
	ImGui::Begin("Vertex Selection");

	ImGui::Checkbox("Enabled", &m_Checked);

	ImGui::End();
	ImGuiIO& io = ImGui::GetIO();

	Ray		mouseRay;
	Camera*	camera = gfx::g_GFXEngine.GetCamera();
	CalculateRayFromPixel(glm::ivec2(io.MousePos.x, io.MousePos.y), glm::ivec2(camera->GetLens().WindowWidth, camera->GetLens().WindowHeight), glm::inverse(camera->GetViewProjection()), &mouseRay);



	return m_Checked;
}

void VerticeSelection::Draw(gfx::RenderQueue* renderQueue, gfx::RenderObject ro){

}

glm::vec3 VerticeSelection::ClosestPointOnFirstRay(const Ray& first, const Ray& second) const {
	const float a = glm::dot(first.Direction, first.Direction);
	const float b = glm::dot(first.Direction, second.Direction);
	const float e = glm::dot(second.Direction, second.Direction);
	const float d = a*e - b*b;

	// if lines are not parallel
	if (d != 0.0f) {
		const glm::vec3	r = first.Position - second.Position;
		const float		c = glm::dot(first.Direction, r);
		const float		f = glm::dot(second.Direction, r);
		const float		s = (b*f - c*e) / d;

		return first.Position + first.Direction * s;
	}
	return first.Position;
}

void VerticeSelection::CalculateRayFromPixel(const glm::ivec2& pixel, const glm::ivec2& windowSize, const glm::mat4& invViewProj, Ray* outRay) const {
	// Clip space coordinates for the pixel. (-1,-1) in lower left corner, (-1,1) upper left corner, (1,-1) lower right corner. 
	const glm::vec2	mousePosNorm = glm::vec2(-1.0f + 2.0f * (pixel.x / static_cast<float>(windowSize.x)),
		1.0f - 2.0f * ((pixel.y) / static_cast<float>(windowSize.y)));

	// Translating pixel at near plane and far plane to world coordinates. Z-coordinate is depth into the screen (values between -1 and 1 are in view of camera).
	const glm::vec4 nearHomogeneous = invViewProj * glm::vec4(mousePosNorm.x, mousePosNorm.y, 0.0f, 1.0f);
	const glm::vec4 farHomogeneous = invViewProj * glm::vec4(mousePosNorm.x, mousePosNorm.y, 1.0f, 1.0f);
	const glm::vec3 nearWorld = glm::vec3(nearHomogeneous) / nearHomogeneous.w;
	const glm::vec3 farWorld = glm::vec3(farHomogeneous) / farHomogeneous.w;

	outRay->Position = nearWorld;
	outRay->Direction = glm::normalize(farWorld - nearWorld);
}

bool VerticeSelection::RayOBB(const Ray* ray, const OBB2* obb, glm::vec3* outIntersectionPoint) const {
	float t_min = -FLT_MAX;
	float t_max = FLT_MAX;

	const glm::vec3 p = obb->Position - ray->Position;

	float e;
	float f;
	float fInv;
	float t_1;
	float t_2;

	for (int i = 0; i < OBB_DIRECTIONS; ++i)
	{
		e = glm::dot(obb->Directions[i], p);
		f = glm::dot(obb->Directions[i], ray->Direction);

		if (f > EPSILON || f < -EPSILON)
		{
			fInv = 1.0f / f;
			t_1 = (e + obb->HalfSizes[i]) * fInv;
			t_2 = (e - obb->HalfSizes[i]) * fInv;

			if (t_1	> t_2)	{ const float temp = t_1; t_1 = t_2; t_2 = temp; }
			if (t_1	> t_min)	{ t_min = t_1; }
			if (t_2	< t_max)	{ t_max = t_2; }
			if (t_min	> t_max)	{ return false; }
			if (t_max	< 0.0f)	{ return false; }
		}
		else if ((-e - obb->HalfSizes[i] > 0.0f) || (-e + obb->HalfSizes[i] < 0.0f)) { return false; }
	}

	float distance = t_min > 0.0f ? t_min : t_max;
	//*outIntersectionPoint = ray->Position + distance * ray->Direction;	// TODOOE: Make this work.
	return true;
}