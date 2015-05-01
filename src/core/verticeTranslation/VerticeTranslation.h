#pragma once

#include <vector>
#include "../../gfx/ModelBank.h"
#include "../../gfx/RenderQueue.h"

void TempSelectVertices( gfx::ModelHandle modelHandle, std::vector<unsigned int>& selectedVertices );	// TODO: Remove when real vertice selection is implemented.

class VerticeTranslation {
public:
	void						Initialize();
	void						Update( const float deltaTime );
	void						Draw( gfx::RenderQueue* renderQueue );

	void						TempSelectVertices( std::vector<unsigned int>& selectedVertices );

	void						SetSelectedVertices( const std::vector<unsigned int>& newSelectedVertices );

private:
	std::vector<unsigned int>	m_SelectedVertices;
	glm::vec3					m_TranslationToolPosition;
	gfx::ModelHandle			m_TranslationToolModel;
};