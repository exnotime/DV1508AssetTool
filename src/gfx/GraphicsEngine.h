#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderQueue.h"
#include "Camera.h"
#include "ShaderBank.h"
#include "FrameBuffer.h"
namespace gfx{

#define g_GFXEngine GraphicsEngine::GetInstance()
	class GraphicsEngine{
	public:
		GraphicsEngine( );
		~GraphicsEngine( );
		static GraphicsEngine& GetInstance( ){ static GraphicsEngine engine; return engine; };
		GLFWwindow* Initialize( int width, int height, bool vsync, bool fullscreen );
		void UpdateWindowSize(int width, int height);
		void Render(RenderQueue* drawQueue);
		void RenderWireFrame(RenderObject ro);
		void Swap( );
		Camera* GetCamera( ) { return &m_Camera; }
		float GetWidth() { return (float)m_Width; }
		float GetHeight(){ return (float)m_Height; }
	private:
		void RenderGeometry(RenderQueue* drawQueue);
		void RenderSprites(RenderQueue* drawQueue);
		void RenderToTexture(RenderQueue* drawQueue);
		void RenderActiveTarget();
		void RenderGizmos(RenderQueue* drawQueue);
		void RenderLines(RenderQueue* drawQueue);

		void BuildLineBuffer();
		GLFWwindow*			m_Window;
		FrameBuffer			m_FrameBuffer;
		Camera				m_Camera;
		ShaderProgramHandle m_Shader;
		ShaderProgramHandle m_SpriteShader;
		ShaderProgramHandle	m_GizmoProgram;
		ShaderProgramHandle	m_LineProgram;
		int					m_Width;
		int					m_Height;
		Texture*			m_SkyTex;
		Texture*			m_IrradianceTex;
		GLuint				m_LineVBO;
		GLuint				m_LineVAO;
		float				m_LineBuffer[100000];
	};
};
