#pragma once
#include "../gfx/GraphicsEngine.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.cpp>
#include <imgui/imgui_impl_glfw_gl3.cpp>
#include "Game.h"
#include "../gfx/RenderQueue.h"
#include "../gfx/ModelBank.h"

void RenderImgui( );
int main( ){

	int width = 1600;
	int height = 900;
	bool vsync = false;
	bool fullscreen = false;

	GLFWwindow* window = gfx::g_GFXEngine.Initialize( width, height, vsync, fullscreen );
	Game game;
	game.Initialize( );
	gfx::g_ModelBank.BuildBuffers( ); //update vertex and index buffer
	gfx::RenderQueue drawQueue;

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
		//render graphics
		gfx::g_GFXEngine.Render( &drawQueue );
		RenderImgui( );
		gfx::g_GFXEngine.Swap( );
		drawQueue.Clear();
	};

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