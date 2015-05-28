#include "VerticeTranslation.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>
#include "../gfx/GraphicsEngine.h"
#include "../gfx/Camera.h"

#define SHOW_IMGUI_VERTICE_TRANSLATION	false
#define EPSILON 1.0e-14f

struct Ray {
	glm::vec3	Position;
	glm::vec3	Direction;
};

// TODO: Remove when real vertice selection is implemented.
void TempSelectVertices( gfx::ModelHandle modelHandle, std::vector<unsigned int>& selectedVertices ) {
	const gfx::Model& model = gfx::g_ModelBank.FetchModel( modelHandle );
	static int meshIndex = 0;
	static int prevMeshIndex = meshIndex + 1;
	static bool unselect = false;

	if ( SHOW_IMGUI_VERTICE_TRANSLATION ) {
		ImGui::Begin( "VerticeTranslation" );
		ImGui::SliderInt( "Mesh", &meshIndex, 0, (int)(model.Meshes.size() - 1));
		ImGui::Checkbox( "Unselect", &unselect );
		ImGui::End();
	}

	if ( unselect )
		selectedVertices.clear();
	else if ( meshIndex != prevMeshIndex ) {
		prevMeshIndex = meshIndex;
		selectedVertices.clear();
		int startIndex = model.VertexHandle + model.Meshes[meshIndex].VertexBufferOffset;
		int endIndex = startIndex + model.Meshes[meshIndex].Size;
		for (int i = startIndex; i < endIndex; ++i) 
		{
			selectedVertices.push_back( i );
		}
	}
}

void VerticeTranslation::Initialize() {
	m_TranslationToolPosition	= glm::vec3( 0.0f );
	m_TranslationToolModel		= gfx::g_ModelBank.LoadModel("asset/UnitArrow/Unit_Arrow.obj");
	m_TranslationType			= TranslationType::None;

	m_VolumeAxisX.Directions[0]	= glm::vec3( 1.0f, 0.0f, 0.0f );
	m_VolumeAxisX.Directions[1]	= glm::vec3( 0.0f, 1.0f, 0.0f );
	m_VolumeAxisX.Directions[2]	= glm::vec3( 0.0f, 0.0f, 1.0f );

	m_VolumeAxisY.Directions[0]	= glm::vec3( 1.0f, 0.0f, 0.0f );
	m_VolumeAxisY.Directions[1]	= glm::vec3( 0.0f, 1.0f, 0.0f );
	m_VolumeAxisY.Directions[2]	= glm::vec3( 0.0f, 0.0f, 1.0f );

	m_VolumeAxisZ.Directions[0]	= glm::vec3( 1.0f, 0.0f, 0.0f );
	m_VolumeAxisZ.Directions[1]	= glm::vec3( 0.0f, 1.0f, 0.0f );
	m_VolumeAxisZ.Directions[2]	= glm::vec3( 0.0f, 0.0f, 1.0f );
}

void VerticeTranslation::Update( const float deltaTime ) {
	if ( m_SelectedVertices.empty() ) {
		return;
	}

	ImGuiIO& io = ImGui::GetIO();

	if ( io.KeyShift ) {
		m_TranslationType = TranslationType::None;
		return;
	}

	Ray line;
	line.Position	= m_TranslationToolPosition;

	Ray		mouseRay;
	Camera*	camera		= gfx::g_GFXEngine.GetCamera();
	CalculateRayFromPixel( glm::ivec2( io.MousePos.x, io.MousePos.y - BUTTON_SIZE), glm::ivec2( camera->GetLens().WindowWidth, camera->GetLens().WindowHeight ), glm::inverse( camera->GetViewProjection() ), &mouseRay );

	if ( io.MouseClicked[0] ) {
		const float hitboxHalfSize = 0.11f;

		m_VolumeAxisX.HalfSizes[0]	= m_TranslationToolScale * 0.5f;
		m_VolumeAxisX.HalfSizes[1]	= m_TranslationToolScale * hitboxHalfSize;
		m_VolumeAxisX.HalfSizes[2]	= m_TranslationToolScale * hitboxHalfSize;

		m_VolumeAxisY.HalfSizes[0]	= m_TranslationToolScale * hitboxHalfSize;
		m_VolumeAxisY.HalfSizes[1]	= m_TranslationToolScale * 0.5f;
		m_VolumeAxisY.HalfSizes[2]	= m_TranslationToolScale * hitboxHalfSize;

		m_VolumeAxisZ.HalfSizes[0]	= m_TranslationToolScale * hitboxHalfSize;
		m_VolumeAxisZ.HalfSizes[1]	= m_TranslationToolScale * hitboxHalfSize;
		m_VolumeAxisZ.HalfSizes[2]	= m_TranslationToolScale * 0.5f;

		m_VolumeAxisX.Position = m_TranslationToolPosition + glm::vec3( m_VolumeAxisX.HalfSizes[0], 0.0f, 0.0f );
		m_VolumeAxisY.Position = m_TranslationToolPosition + glm::vec3( 0.0f, m_VolumeAxisY.HalfSizes[1], 0.0f );
		m_VolumeAxisZ.Position = m_TranslationToolPosition + glm::vec3( 0.0f, 0.0f, m_VolumeAxisZ.HalfSizes[2] );

		glm::vec3 intersectionPoint;
		if ( RayOBB( &mouseRay, &m_VolumeAxisX, &intersectionPoint ) ) {
			m_TranslationType = TranslationType::X;
			line.Direction	= glm::vec3( 1.0f, 0.0f, 0.0f );
			m_TranslationToolOffset = ClosestPointOnFirstRay( line, mouseRay ) - m_TranslationToolPosition;
		} else if ( RayOBB( &mouseRay, &m_VolumeAxisY, &intersectionPoint ) ) {
			m_TranslationType = TranslationType::Y;
			line.Direction	= glm::vec3( 0.0f, 1.0f, 0.0f );
			m_TranslationToolOffset = ClosestPointOnFirstRay( line, mouseRay ) - m_TranslationToolPosition;
		} else if ( RayOBB( &mouseRay, &m_VolumeAxisZ, &intersectionPoint ) ) {
			m_TranslationType = TranslationType::Z;
			line.Direction	= glm::vec3( 0.0f, 0.0f, 1.0f );
			m_TranslationToolOffset = ClosestPointOnFirstRay( line, mouseRay ) - m_TranslationToolPosition;
		}
		m_TranslationToolOffset /= m_TranslationToolScale;
		m_TranslatingDirection = line.Direction;
	}

	std::vector<gfx::VertexPosNormalTexTangent>& vertices = gfx::g_ModelBank.GetVertices();

	if ( m_TranslationType != TranslationType::None && io.MouseDown[0] ) {
		line.Direction = m_TranslatingDirection;
		const glm::vec3 diff = ClosestPointOnFirstRay( line, mouseRay ) - (m_TranslationToolPosition + (m_TranslationToolOffset * m_TranslationToolScale));

		if ( diff != glm::vec3( 0.0f ) ) {
			glm::vec4 diffVec4 = glm::vec4( diff, 0.0f );
			for ( auto& index : m_SelectedVertices ) {
				vertices[index].Position +=  diffVec4;
			}
			gfx::g_ModelBank.BuildBuffers();
		}
	} else {
		m_TranslationType = TranslationType::None;
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

	glm::mat4x4 viewProj			= gfx::g_GFXEngine.GetCamera()->GetViewProjection();
	glm::mat4x4 invViewProj			= glm::inverse( viewProj );

	glm::vec4 nearHomogeneous		= viewProj * glm::vec4( m_TranslationToolPosition, 1.0f );
	nearHomogeneous					/= nearHomogeneous.w;
	const glm::vec4 farHomogeneous	= invViewProj * glm::vec4( nearHomogeneous.x + 0.15f, nearHomogeneous.y, nearHomogeneous.z, 1.0f );
	const glm::vec3 farWorld		= glm::vec3( farHomogeneous ) / farHomogeneous.w;

	m_TranslationToolScale			= glm::distance( farWorld, m_TranslationToolPosition );

	const float			halfPi				= 0.5f * glm::pi<float>();
	const glm::mat4x4	scaleTranslation	= glm::translate( m_TranslationToolPosition ) * glm::scale( glm::vec3(m_TranslationToolScale) );

	gfx::GizmoObject renderObject;
	renderObject.Model	= m_TranslationToolModel;
	float alpha = m_TranslationType == TranslationType::None ? 0.7f : 0.3f;
	if ( m_TranslationType == TranslationType::None || m_TranslationType == TranslationType::X ) {
		renderObject.world	= scaleTranslation * glm::rotate( -halfPi, glm::vec3(0.0f, 0.0f, 1.0f) );	// X
		renderObject.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * alpha;
		renderQueue->Enqueue(renderObject);
	}

	if ( m_TranslationType == TranslationType::None || m_TranslationType == TranslationType::Y ) {
		renderObject.world	= scaleTranslation;	// Y
		renderObject.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * alpha;
		renderQueue->Enqueue(renderObject);
	}

	if ( m_TranslationType == TranslationType::None || m_TranslationType == TranslationType::Z ) {
		renderObject.world	= scaleTranslation * glm::rotate( halfPi, glm::vec3(1.0f, 0.0f, 0.0f) );	// Z
		renderObject.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * alpha;
		renderQueue->Enqueue(renderObject);
	}
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
											1.0f - 2.0f * ((pixel.y) / static_cast<float>(windowSize.y)) );

	// Translating pixel at near plane and far plane to world coordinates. Z-coordinate is depth into the screen (values between -1 and 1 are in view of camera).
	const glm::vec4 nearHomogeneous	= invViewProj * glm::vec4( mousePosNorm.x, mousePosNorm.y, 0.0f, 1.0f );
	const glm::vec4 farHomogeneous	= invViewProj * glm::vec4( mousePosNorm.x, mousePosNorm.y, 1.0f, 1.0f );
	const glm::vec3 nearWorld		= glm::vec3( nearHomogeneous ) / nearHomogeneous.w;
	const glm::vec3 farWorld		= glm::vec3( farHomogeneous ) / farHomogeneous.w;

	outRay->Position			= nearWorld;
	outRay->Direction			= glm::normalize( farWorld - nearWorld );
}

bool VerticeTranslation::RayOBB( const Ray* ray, const OBB* obb, glm::vec3* outIntersectionPoint ) const {
	float t_min = -FLT_MAX;
	float t_max =  FLT_MAX;

	const glm::vec3 p = obb->Position - ray->Position;

	float e;
	float f;
	float fInv;
	float t_1;
	float t_2;

	for ( int i = 0; i < OBB_DIRECTIONS; ++i )
	{
		e	= glm::dot( obb->Directions[i], p );
		f	= glm::dot( obb->Directions[i], ray->Direction );

		if ( f > EPSILON || f < -EPSILON )
		{
			fInv	= 1.0f / f;
			t_1 = (e + obb->HalfSizes[i]) * fInv;
			t_2 = (e - obb->HalfSizes[i]) * fInv;

			if ( t_1	> t_2	)	{ const float temp = t_1; t_1 = t_2; t_2 = temp; }
			if ( t_1	> t_min )	{ t_min = t_1; }
			if ( t_2	< t_max )	{ t_max = t_2; }
			if ( t_min	> t_max )	{ return false; }
			if ( t_max	< 0.0f	)	{ return false; }
		}
		else if ( ( -e - obb->HalfSizes[i] > 0.0f) || ( -e + obb->HalfSizes[i] < 0.0f ) ) { return false; }
	}

	float distance = t_min > 0.0f ? t_min : t_max;
	//*outIntersectionPoint = ray->Position + distance * ray->Direction;	// TODOOE: Make this work.
	return true;
}