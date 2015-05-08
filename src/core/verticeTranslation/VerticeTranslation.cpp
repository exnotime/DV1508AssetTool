#include "VerticeTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

// TODO: Remove when real vertice selection is implemented.
void TempSelectVertices( gfx::ModelHandle modelHandle, std::vector<unsigned int>& selectedVertices ) {
	const gfx::Model& model = gfx::g_ModelBank.FetchModel( modelHandle );
	static int meshIndex = 2;
	static int prevMeshIndex = meshIndex + 1;

	ImGui::Begin( "VerticeTranslation" );
	ImGui::SliderInt( "Mesh", &meshIndex, 0, model.Meshes.size() - 1 );
	ImGui::End();

	if ( meshIndex != prevMeshIndex ) {
		prevMeshIndex = meshIndex;
		selectedVertices.clear();
		int startIndex = model.VertexHandle + model.Meshes[meshIndex].VertexBufferOffset;
		int endIndex = startIndex + model.Meshes[meshIndex].Size;
		for ( unsigned int i = startIndex; i < endIndex; ++i ) {
			selectedVertices.push_back( i );
		}
	}
}

void VerticeTranslation::Initialize() {
	m_TranslationToolPosition	= glm::vec3( 0.0f );
	m_TranslationToolModel		= gfx::g_ModelBank.LoadModel("asset/UnitArrow/Unit_Arrow.obj");
}

void VerticeTranslation::Update( const float deltaTime ) {
	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	glm::vec3 avaragePosition( 0.0f );
	for ( auto& index : m_SelectedVertices ) {
		avaragePosition += glm::vec3(vertices[index].Position);
	}
	avaragePosition /= m_SelectedVertices.size();
	m_TranslationToolPosition = avaragePosition;

	static float derp = 0.0f;
	if ( (derp -= deltaTime) <= 0.0f ) {
		derp = 0.1f;
		gfx::g_ModelBank.BuildBuffers();
	}
}

void VerticeTranslation::Draw( gfx::RenderQueue* renderQueue ) {
	const float			halfPi				= 0.5f * glm::pi<float>();
	const glm::mat4x4	scaleTranslation	= glm::translate( m_TranslationToolPosition ) * glm::scale( glm::vec3(1.0f) );

	gfx::RenderObject renderObject;
	renderObject.Model	= m_TranslationToolModel;

	renderObject.world	= scaleTranslation * glm::rotate( -halfPi, glm::vec3(0.0f, 0.0f, 1.0f) );	// X
	renderQueue->Enqueue(renderObject);

	renderObject.world	= scaleTranslation;	// Y
	renderQueue->Enqueue(renderObject);

	renderObject.world	= scaleTranslation * glm::rotate( halfPi, glm::vec3(1.0f, 0.0f, 0.0f) );	// Z
	renderQueue->Enqueue(renderObject);
}

void VerticeTranslation::SetSelectedVertices( const std::vector<unsigned int>& newSelectedVertices ) {
	m_SelectedVertices = newSelectedVertices;
}