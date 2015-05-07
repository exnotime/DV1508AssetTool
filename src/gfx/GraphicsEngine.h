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
		void Swap( );
		Camera* GetCamera( ) { return &m_Camera; };
		float GetWidth() { return m_Width; }
		float GetHeight(){ return m_Height; }
	private:
		void RenderGeometry(RenderQueue* drawQueue);
		void RenderSprites(RenderQueue* drawQueue);
		void RenderToTexture(RenderQueue* drawQueue);
		void RenderActiveTarget();

		GLFWwindow*			m_Window;
		FrameBuffer			m_FrameBuffer;
		Camera				m_Camera;
		ShaderProgramHandle m_Shader;
		ShaderProgramHandle m_SpriteShader;
		int					m_Width;
		int					m_Height;
		Texture*			m_SkyTex;
		Texture*			m_IrradianceTex;
		Texture*			m_TestTex;
	};
};
