#pragma once
#include "../gfx/GraphicsEngine.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <imgui/imgui.cpp>
#include <imgui/imgui_impl_glfw_gl3.cpp>
#include "Game.h"
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"

void RenderImgui( );
void UpdateWindow(GLFWwindow* window, int w, int h);
int main( ){

	int width = 1280;
	int height = 770;
	bool vsync = true;
	bool fullscreen = false;

	GLFWwindow* window = gfx::g_GFXEngine.Initialize( width, height, vsync, fullscreen );
	glfwSetFramebufferSizeCallback(window, UpdateWindow);
	
	Game game;
	game.Initialize( width, height);
	gfx::g_ModelBank.BuildBuffers( ); //update vertex and index buffer
	gfx::RenderQueue drawQueue;
	gfx::RenderObject wireFrameRO;

	while( true ){
		//update input and window
		ImGuiIO& io = ImGui::GetIO( );
		glfwPollEvents( );
		ImGui_ImplGlfwGL3_NewFrame( );
		if( glfwWindowShouldClose( window ) ){
			break;
		}
		//update game
		game.Update( io.DeltaTime );
		game.Render( &drawQueue );
		wireFrameRO = game.GetWireFrameModel();
		//render graphics
		gfx::g_GFXEngine.Render( &drawQueue );
		if (game.GetWireFrameState()){
			gfx::g_GFXEngine.RenderWireFrame(wireFrameRO);
		}
		RenderImgui( );
		gfx::g_GFXEngine.Swap( );
		std::stringstream ss;
		ss << "AssetTool FPS:" << io.Framerate;
		glfwSetWindowTitle(window, ss.str().c_str());
		drawQueue.Clear();
	};

	game.Shutdown();
	ImGui_ImplGlfwGL3_Shutdown( );
	glfwTerminate( );
	return 0;
}

void RenderImgui( ){
	ImGuiIO& io = ImGui::GetIO( );
	glViewport( 0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y );
	//glClear( GL_COLOR_BUFFER_BIT );
	ImGui::Render( );
}

void UpdateWindow(GLFWwindow* window, int w, int h){
	gfx::g_GFXEngine.UpdateWindowSize(w, h);
}