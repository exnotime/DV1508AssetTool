#pragma once

#include <vector>
#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

#define OBB_DIRECTIONS 3

enum class TranslationType {
	X,
	Y,
	Z,
	XY,
	XZ,
	YZ,
	None
};

struct Ray;

struct OBB {
	glm::vec3	Position;
	glm::vec3	Directions	[OBB_DIRECTIONS];	// Should be normalized.
	float		HalfSizes	[OBB_DIRECTIONS];	// Half length of their respective directions.
};

void TempSelectVertices( gfx::ModelHandle modelHandle, std::vector<unsigned int>& selectedVertices );	// TODO: Remove when real vertice selection is implemented.

class VerticeTranslation {
public:
	void						Initialize();
	void						Update( const float deltaTime );
	void						Draw( gfx::RenderQueue* renderQueue );

	void						TempSelectVertices( std::vector<unsigned int>& selectedVertices );

	void						SetSelectedVertices( const std::vector<unsigned int>& newSelectedVertices );

private:
	glm::vec3					ClosestPointOnFirstRay( const Ray& first, const Ray& second ) const;
	void						CalculateRayFromPixel( const glm::ivec2& pixel, const glm::ivec2& windowSize, const glm::mat4& invViewProj, Ray* outRay ) const;
	bool						RayOBB( const Ray* rayVolume, const OBB* obbVolume, glm::vec3* outIntersectionPoint ) const;

	std::vector<unsigned int>	m_SelectedVertices;
	glm::vec3					m_TranslationToolPosition;
	glm::vec3					m_TranslationToolOffset;
	float						m_TranslationToolScale;
	gfx::ModelHandle			m_TranslationToolModel;
	TranslationType				m_TranslationType;
	glm::vec3					m_TranslatingDirection;
	OBB							m_VolumeAxisX;
	OBB							m_VolumeAxisY;
	OBB							m_VolumeAxisZ;
	OBB							m_VolumeAxisXY;
	OBB							m_VolumeAxisXZ;
	OBB							m_VolumeAxisYZ;
};