#include "GraphicsEngine.h"
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/imgui.h>
#include <glm/gtx/transform.hpp>
#include "MaterialBank.h"
#include "Material.h"

gfx::GraphicsEngine::GraphicsEngine(){

}

gfx::GraphicsEngine::~GraphicsEngine(){
	glfwDestroyWindow( m_Window );
	
}

GLFWwindow* gfx::GraphicsEngine::Initialize( int width, int height, bool vsync, bool fullscreen ){
	m_Width = width;
	m_Height = height;
	
	if( !glfwInit( ) )
		return nullptr;
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	

	if(vsync){
		glfwSwapInterval( 1 );
	}else{
		glfwSwapInterval( 0 );
	}
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//set camera
	float aspectRatio = width / (float)height;
	m_Camera.GetEditableLens( ).Near = 0.1f;
	m_Camera.GetEditableLens( ).Far = 100.0f;
	m_Camera.GetEditableLens( ).VerticalFOV = ( ( 90.0f / ( aspectRatio ) ) / 360.0f ) * 2 * glm::pi<float>( ); // calc FOV as horisontal FOV 90 degrees
	m_Camera.GetEditableLens( ).WindowHeight = height;
	m_Camera.GetEditableLens( ).WindowWidth = (int)(width * 0.5f);
	m_Camera.SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));

	m_Camera.CalculateViewProjection();
	//load shaders
	m_Shader = g_ShaderBank.LoadShaderProgram( "shader/CombinedShader.glsl" );
	m_SpriteShader = g_ShaderBank.LoadShaderProgram("shader/SpriteShader.glsl");
	m_GizmoProgram = g_ShaderBank.LoadShaderProgram("shader/GizmoProgram.glsl");
	//Test Texture
	m_TestTex = new Texture();
	m_TestTex->Init("asset/rockman_teeth.png", TEXTURE_COLOR);
	//Load cubeTex
	m_SkyTex = new Texture();
	m_SkyTex->Init("asset/CubeMaps/square.dds", TEXTURE_CUBE);
	m_IrradianceTex = new Texture();
	m_IrradianceTex->Init("asset/CubeMaps/square_irr.dds", TEXTURE_CUBE);

	m_FrameBuffer.Init();
	return m_Window;
}

void gfx::GraphicsEngine::UpdateWindowSize(int width, int height){
	m_Width = width;
	m_Height = height;
}

void gfx::GraphicsEngine::Render( RenderQueue* drawQueue ){
	m_Camera.CalculateViewProjection();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	TextureHandle target = drawQueue->GetTargetTexture();
	m_FrameBuffer.SetTexture(target);
	RenderToTexture(drawQueue);
	RenderActiveTarget();
	RenderGeometry(drawQueue);
	RenderGizmos(drawQueue);
	RenderSprites(drawQueue);
	
	glUseProgram(0);
}

void gfx::GraphicsEngine::RenderGeometry(RenderQueue* drawQueue){
	glViewport(0, 0, (GLsizei)(m_Width * 0.5f), (GLsizei)m_Height);
	//glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	g_ModelBank.ApplyBuffers();
	ShaderProgram* prog = g_ShaderBank.GetProgramFromHandle(m_Shader);
	prog->Apply();
	prog->SetUniformMat4("g_ViewProj", m_Camera.GetViewProjection());
	prog->SetUniformVec3("g_Campos", m_Camera.GetPosition());
	static float metal = 0.001f;
	static float roughness = 0.001f;
	glm::vec3 lightDir = glm::vec3(0.5f,-1,0.5f);
	static float lightangle = 4.0f;
	ImGui::Begin("Lighting");
	ImGui::SliderFloat("Roughness", &roughness, 0.001f, 1);
	ImGui::SliderFloat("Metallic", &metal, 0.001f, 1);
	ImGui::SliderFloat("LightDir", &lightangle, 0, 2 * glm::pi<float>());
	if (ImGui::Button("Recompile Shader")){
		g_ShaderBank.RecompileAllShaders();
	}
	ImGui::End();
	prog->SetUniformFloat("g_Roughness", roughness);
	prog->SetUniformFloat("g_Metallic", metal);

	glm::vec4 temp = glm::vec4(lightDir, 0) * glm::rotate(lightangle, glm::vec3(0, 1, 0));
	prog->SetUniformVec3("g_LightDir", glm::vec3(temp.x, temp.y, temp.z));
	prog->SetUniformVec3("g_CamDir", m_Camera.GetForward());
	m_SkyTex->Apply(prog->FetchUniform("g_SkyCubeTex"), 1);
	m_IrradianceTex->Apply(prog->FetchUniform("g_IrradianceCubeTex"), 2);

	for (auto& object : drawQueue->GetQueue()){
		Model model = g_ModelBank.FetchModel(object.Model);
		prog->SetUniformMat4("g_World", object.world);
		for (auto& mesh : model.Meshes){

			Material* mat = g_MaterialBank.GetMaterial(model.MaterialOffset + mesh.Material);
			Texture* albedoTex = g_MaterialBank.GetTexture(mat->GetAlbedoTexture());
			Texture* normalTex = g_MaterialBank.GetTexture(mat->GetNormalTexture());
			Texture* roughnessTex = g_MaterialBank.GetTexture(mat->GetRoughnessTexture());
			prog->SetUniformTextureHandle("g_DiffuseTex", albedoTex->GetHandle(), 0);
			prog->SetUniformTextureHandle("g_NormalTex", normalTex->GetHandle(), 3);
			prog->SetUniformTextureHandle("g_RoughnessTex", roughnessTex->GetHandle(), 4);
			glDrawElements(GL_TRIANGLES, mesh.Indices, GL_UNSIGNED_INT,
				(GLvoid*)(0 + ((model.IndexHandle + mesh.IndexBufferOffset) * sizeof(unsigned int))));
		}
	}
}

void gfx::GraphicsEngine::RenderSprites(RenderQueue* drawQueue){
	glViewport(0, 0, m_Width, m_Height);
	//Render Sprites
	ShaderProgram* spriteProg = g_ShaderBank.GetProgramFromHandle(m_SpriteShader);
	spriteProg->Apply();
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int layer = 0; layer < 3; layer++){
		for (auto& spr : drawQueue->GetSpriteQueue()[layer]){
			spriteProg->SetUniformVec4("g_Pos", spr.GetPosFlt());
			spriteProg->SetUniformVec4("g_Size", spr.GetSizeFlt());
			spriteProg->SetUniformVec4("g_Color", spr.GetColor());
			g_MaterialBank.GetTexture(spr.GetTexture())->Apply(spriteProg->FetchUniform("g_Texture"), 0);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
}
void gfx::GraphicsEngine::RenderToTexture(RenderQueue* drawQueue){
	TextureHandle target = drawQueue->GetTargetTexture();
	m_FrameBuffer.SetTexture(target);
	m_FrameBuffer.Apply();

	ShaderProgram* spriteProg = g_ShaderBank.GetProgramFromHandle(m_SpriteShader);
	spriteProg->Apply();
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	static bool additive = true; 
	ImGui::Checkbox("Additive", &additive);
	if (additive){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else {
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}
	static glm::vec4 color = glm::vec4(1);
	ImGui::ColorEdit4("BrushColor", &color[0], true);

	for (auto& brush : drawQueue->GetBrushQueue()){
		Texture* brushTex = g_MaterialBank.GetTexture(brush.Texture);
		glm::vec2 brushSize(brush.Size / (m_Width * 0.5f), brush.Size / m_Height);

		spriteProg->SetUniformVec4("g_Pos", glm::vec4(glm::vec2(brush.Position.x - brushSize.x * 0.5f, 1.0f - brush.Position.y + brushSize.y * 0.5f), 0, 0));
		spriteProg->SetUniformVec4("g_Size", glm::vec4(brushSize.x, brushSize.y, 1, 1));
		spriteProg->SetUniformVec4("g_Color", color);
		brushTex->Apply(spriteProg->FetchUniform("g_Texture"), 0);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gfx::GraphicsEngine::RenderActiveTarget(){
	glViewport((GLint)(m_Width * 0.5f), 0, (GLint)(m_Width * 0.5f), m_Height);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ShaderProgram* spriteProg = g_ShaderBank.GetProgramFromHandle(m_SpriteShader);
	spriteProg->Apply();
	glBindVertexArray(0);
	Texture* tex = g_MaterialBank.GetTexture(m_FrameBuffer.GetTexture());
	float sizeH;
	sizeH = tex->GetHeight() / tex->GetWidth();
	tex->Apply(spriteProg->FetchUniform("g_Texture"), 0);
	spriteProg->SetUniformVec4("g_Color", glm::vec4(1));
	spriteProg->SetUniformVec4("g_Pos", glm::vec4(0.0f, 0.5f + sizeH * 0.5f, 0.0f,0.0f));
	spriteProg->SetUniformVec4("g_Size", glm::vec4(1.0f, sizeH, 1.0f, 1.0f));
	glDrawArrays(GL_POINTS, 0, 1);

}

void gfx::GraphicsEngine::Swap( ){
	glfwSwapBuffers( m_Window );

}

void gfx::GraphicsEngine::RenderWireFrame(RenderObject ro){
	glViewport(0, 0, (GLsizei)(m_Width * 0.5f), (GLsizei)m_Height);
	glDepthFunc(GL_EQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	g_ModelBank.ApplyBuffers();
	ShaderProgram* prog = g_ShaderBank.GetProgramFromHandle(m_Shader);
	prog->Apply();
	prog->SetUniformMat4("g_ViewProj", m_Camera.GetViewProjection());
	prog->SetUniformVec3("g_Campos", m_Camera.GetPosition());

	Model model = g_ModelBank.FetchModel(ro.Model);
	prog->SetUniformMat4("g_World", ro.world);
	for (auto& mesh : model.Meshes){
		glDrawElements(GL_TRIANGLES, mesh.Indices, GL_UNSIGNED_INT,
			(GLvoid*)(0 + ((model.IndexHandle + mesh.IndexBufferOffset) * sizeof(unsigned int))));
	}

	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void gfx::GraphicsEngine::RenderGizmos(RenderQueue* drawQueue){
	ShaderProgram* prog = g_ShaderBank.GetProgramFromHandle(m_GizmoProgram);
	prog->Apply();
	prog->SetUniformMat4("g_ViewProj", m_Camera.GetViewProjection());
	glDisable(GL_DEPTH_TEST);
	for (auto& gizmo : drawQueue->GetGizmoQueue()){
		Model model = g_ModelBank.FetchModel(gizmo.Model);
		prog->SetUniformMat4("g_World", gizmo.world);
		for (auto& mesh : model.Meshes){
			prog->SetUniformVec4("g_Color", gizmo.Color);
			glDrawElements(GL_TRIANGLES, mesh.Indices, GL_UNSIGNED_INT,
				(GLvoid*)(0 + ((model.IndexHandle + mesh.IndexBufferOffset) * sizeof(unsigned int))));
		}
	}
	glEnable(GL_DEPTH_TEST);
}