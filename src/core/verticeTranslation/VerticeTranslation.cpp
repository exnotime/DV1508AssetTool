#include "VerticeTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/GraphicsEngine.h"
#include "../gfx/Camera.h"

struct Ray {
	glm::vec3	Position;
	glm::vec3	Direction;
};

// TODO: Remove when real vertice selection is implemented.
void TempSelectVertices( gfx::ModelHandle modelHandle, std::vector<unsigned int>& selectedVertices ) {
	const gfx::Model& model = gfx::g_ModelBank.FetchModel( modelHandle );
	static int meshIndex = 2;
	static int prevMeshIndex = meshIndex + 1;
	static bool unselect = true;

	ImGui::Begin( "VerticeTranslation" );
	ImGui::SliderInt( "Mesh", &meshIndex, 0, model.Meshes.size() - 1 );
	ImGui::Checkbox( "Unselect", &unselect );
	ImGui::End();

	if ( unselect )
		selectedVertices.clear();
	else if ( meshIndex != prevMeshIndex ) {
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
	if ( m_SelectedVertices.empty() ) {
		return;
	}

	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	ImGuiIO& io = ImGui::GetIO();
	if ( io.MouseDown[0] ) {
		Ray lineX;
		lineX.Position	= m_TranslationToolPosition;
		lineX.Direction	= glm::vec3( 1.0f, 0.0f, 0.0f );

		Ray		mouseRay;
		Camera*	camera		= gfx::g_GFXEngine.GetCamera();
		CalculateRayFromPixel( glm::ivec2( io.MousePos.x, io.MousePos.y ), glm::ivec2( camera->GetLens().WindowWidth, camera->GetLens().WindowHeight ), glm::inverse( camera->GetViewProjection() ), &mouseRay );

		const glm::vec3 diff = ClosestPointOnFirstRay( lineX, mouseRay ) - m_TranslationToolPosition;

		if ( diff != glm::vec3( 0.0f ) ) {
			glm::vec4 diffVec4 = glm::vec4( diff, 0.0f );
			for ( auto& index : m_SelectedVertices ) {
				vertices[index].Position +=  diffVec4;
			}
			gfx::g_ModelBank.BuildBuffers();
		}
	}

	glm::vec3 avaragePosition( 0.0f );
	for ( auto& index : m_SelectedVertices ) {
		avaragePosition += glm::vec3(vertices[index].Position);
	}
	avaragePosition /= m_SelectedVertices.size();
	m_TranslationToolPosition = avaragePosition;
}

void VerticeTranslation::Draw( gfx::RenderQueue* renderQueue ) {
	if ( m_SelectedVertices.empty() ) {
		return;
	}

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

glm::vec3 VerticeTranslation::ClosestPointOnFirstRay( const Ray& first, const Ray& second ) const {
	const float a = glm::dot( first.Direction, first.Direction );
	const float b = glm::dot( first.Direction, second.Direction );
	const float e = glm::dot( second.Direction, second.Direction );   
	const float d = a*e - b*b;
        
	// if lines are not parallel
	if (d != 0.0f) {
		const glm::vec3	r = first.Position - second.Position;
		const float		c = glm::dot( first.Direction, r );
		const float		f = glm::dot( second.Direction, r );
		const float		s = (b*f - c*e) / d;

		return first.Position + first.Direction * s;
	}
	return first.Position;
}

void VerticeTranslation::CalculateRayFromPixel( const glm::ivec2& pixel, const glm::ivec2& windowSize, const glm::mat4& invViewProj, Ray* outRay ) const {
	// Clip space coordinates for the pixel. (-1,-1) in lower left corner, (-1,1) upper left corner, (1,-1) lower right corner. 
	const glm::vec2	mousePosNorm	= glm::vec2( -1.0f + 2.0f * (pixel.x / static_cast<float>(windowSize.x)),
											1.0f - 2.0f * (pixel.y / static_cast<float>(windowSize.y)) );

	// Translating pixel at near plane and far plane to world coordinates. Z-coordinate is depth into the screen (values between -1 and 1 are in view of camera).
	const glm::vec4 nearHomogeneous	= invViewProj * glm::vec4( mousePosNorm.x, mousePosNorm.y, 0.0f, 1.0f );
	const glm::vec4 farHomogeneous	= invViewProj * glm::vec4( mousePosNorm.x, mousePosNorm.y, 1.0f, 1.0f );
	const glm::vec3 nearWorld		= glm::vec3( nearHomogeneous ) / nearHomogeneous.w;
	const glm::vec3 farWorld		= glm::vec3( farHomogeneous ) / farHomogeneous.w;

	outRay->Position			= nearWorld;
	outRay->Direction			= glm::normalize( farWorld - nearWorld );
}