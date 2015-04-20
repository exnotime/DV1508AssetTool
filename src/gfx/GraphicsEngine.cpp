#include "GraphicsEngine.h"
#include <imgui/imgui_impl_glfw_gl3.h>
#include "MaterialBank.h"
#include "Material.h"
gfx::GraphicsEngine::GraphicsEngine(){

}

gfx::GraphicsEngine::~GraphicsEngine(){
	glfwDestroyWindow( m_Window );
	
}

GLFWwindow* gfx::GraphicsEngine::Initialize( int width, int height, bool vsync, bool fullscreen ){
	if( !glfwInit( ) )
		return nullptr;

	if(fullscreen){
		m_Window = glfwCreateWindow( width, height, "ModelViewer", glfwGetPrimaryMonitor( ), nullptr );
	}else{
		m_Window = glfwCreateWindow( width, height, "ModelViewer", nullptr, nullptr );
	}
	if(!m_Window)
		return nullptr;

	glfwMakeContextCurrent( m_Window );
	ImGui_ImplGlfwGL3_Init( m_Window, true );
	glewExperimental = GL_TRUE;
	glewInit( );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	if(vsync){
		glfwSwapInterval( 1 );
	}else{
		glfwSwapInterval( 0 );
	}
	glFrontFace( GL_CCW );
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//set camera
	float aspectRatio = width / (float)height;
	m_Camera.GetEditableLens( ).Near = 0.1f;
	m_Camera.GetEditableLens( ).Far = 100.0f;
	m_Camera.GetEditableLens( ).VerticalFOV = 0.959931089f; // ( ( 90.0f / ( aspectRatio ) ) / 360.0f ) * 2 * glm::pi<float>( ); // calc FOV as horisontal FOV 90 degrees
	m_Camera.GetEditableLens( ).WindowHeight = height;
	m_Camera.GetEditableLens( ).WindowWidth = width;
	m_Camera.SetPosition(glm::vec3(0.0f));

	m_Camera.CalculateViewProjection();
	//load shaders
	m_Shader = g_ShaderBank.LoadShaderProgram( "shader/CombinedShader.glsl" );

	return m_Window;
}
void gfx::GraphicsEngine::Render( RenderQueue* drawQueue ){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	g_ModelBank.ApplyBuffers();
	ShaderProgram* prog = g_ShaderBank.GetProgramFromHandle(m_Shader);
	prog->Apply();
	prog->SetUniformMat4("g_ViewProj", m_Camera.GetViewProjection());
	for(auto& object : drawQueue->GetQueue()){
		Model model = g_ModelBank.FetchModel(object.Model);
		prog->SetUniformMat4( "g_World", object.world );

		for(auto& mesh : model.Meshes){
			Material* mat = g_MaterialBank.GetMaterial(model.MaterialOffset + mesh.Material);
			prog->SetUniformTextureHandle( "g_DiffuseTex", mat->GetAlbedoTexture()->GetHandle() ,0);
			
			glDrawElements( GL_TRIANGLES, mesh.Indices, GL_UNSIGNED_INT,
				(GLvoid*)( 0 + ( ( model.IndexHandle + mesh.IndexBufferOffset ) * sizeof( unsigned int ) ) ) );
		}

	}
	glUseProgram( 0 );
}

void gfx::GraphicsEngine::Swap( ){
	glfwSwapBuffers( m_Window );
}