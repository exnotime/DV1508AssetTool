#pragma once

#include <vector>
#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

#define OBB_DIRECTIONS 3

struct Ray;

struct OBB2 {
	glm::vec3	Position;
	glm::vec3	Directions[OBB_DIRECTIONS];	// Should be normalized.
	float		HalfSizes[OBB_DIRECTIONS];	// Half length of their respective directions.
};

class VerticeSelection{
public:
	VerticeSelection();
	~VerticeSelection();

	void					Initialize();
	bool					Update(const float deltaTime);
	void					Draw(gfx::RenderQueue* renderQueue, gfx::RenderObject ro );

private:
	std::vector<unsigned int>	m_SelectedVertices;
	bool m_Checked;

	glm::vec3					ClosestPointOnFirstRay(const Ray& first, const Ray& second) const;
	void						CalculateRayFromPixel(const glm::ivec2& pixel, const glm::ivec2& windowSize, const glm::mat4& invViewProj, Ray* outRay) const;
	bool						RayOBB(const Ray* rayVolume, const OBB2* obbVolume, glm::vec3* outIntersectionPoint) const;
};