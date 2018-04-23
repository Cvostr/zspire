
#define _CRT_SECURE_NO_WARNINGS

#include "imgui.h"
#include "../includes/imgui_impl_sdl_gl3.h"

#include <SDL.h>

#include <glew.h>

#include "stdio.h"

#include "../includes/zs-math.h"

#include "../includes/scene_loader.h"
#include "../includes/startup_window.h"
#include "../includes/objects_window.h"
#include "../includes/property_inspector.h"
#include "../includes/file_window.h"

#include "../includes/zs-shader.h"
#include "../includes/Renderer.h"

#include "../includes/zs-camera.h"

#include "../includes/resources_window.h"

float cam_pitch = 0;
float cam_yaw = 0;

float cam_speed = 0.2f;

bool isMouseRelative = false;

ZSpire::Shader obj_shader;

int main(int argc, char* argv[])
{
	ZSpire::InitializeCamera();

	ZSpire::setCameraFOV(45.0f);
	ZSpire::setCameraProjectionType(CAMERA_PROJECTION_PERSPECTIVE);
	ZSpire::setCameraProjectionResolution(1280.0f, 720.0f);

	ZSpire::updateCameraMatrix();

	int WIDTH;
	int HEIGHT;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	FILE* config_file = fopen("editor.ini", "r");

	while (true) {
		char pr[20];
		int e = fscanf(config_file, "%s", pr);

		if (e == EOF) break;

		if (strcmp(pr, "window_width") == 0) {
			fscanf(config_file, "%d", &WIDTH);
		}

		if (strcmp(pr, "window_height") == 0) {
			fscanf(config_file, "%d", &HEIGHT);
		}

	}

	fclose(config_file);
	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window *window = SDL_CreateWindow("ZSpire Scene Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{//GLEW instance creation failed!
	
	}
	
	glEnable(GL_DEPTH_TEST);

	ZSpire::createPlane2D();
	InitializePlaneResource();

	obj_shader.InitializeShader();
	obj_shader.compileFromFile("shaders/object.vs", "shaders/object.fs");
	ZSpire::Renderer::setObjectShaderPtr(&obj_shader);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSdlGL3_Init(window);
	
	ImGui::StyleColorsDark();


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
	
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_LALT) {
					if (isMouseRelative == false) {
						isMouseRelative = true;
						SDL_SetRelativeMouseMode(SDL_TRUE);
					}
					else {
						SDL_SetRelativeMouseMode(SDL_FALSE);
						isMouseRelative = false;
					}
				}

				if (event.key.keysym.sym == SDLK_w) {
					ZSpire::setCameraPosition(ZSpire::getCameraPos() + ZSpire::getCameraFront() * cam_speed);
				}
				if (event.key.keysym.sym == SDLK_s) {
					ZSpire::setCameraPosition(ZSpire::getCameraPos() - ZSpire::getCameraFront() * cam_speed);
				}
				if (event.key.keysym.sym == SDLK_a) {
					ZSpire::setCameraPosition(ZSpire::getCameraPos() - ZSpire::getCameraRight() * cam_speed);
				}
				if (event.key.keysym.sym == SDLK_d) {
					ZSpire::setCameraPosition(ZSpire::getCameraPos() + ZSpire::getCameraRight() * cam_speed);
				}
			
			}
				
			

			if (event.type == SDL_MOUSEMOTION) {
				if (isMouseRelative == true) {
					cam_yaw += event.motion.xrel * 0.06f;
					cam_pitch += event.motion.yrel * 0.06f;

						if (cam_pitch > 89.0f)
							cam_pitch = 89.0f;
						if (cam_pitch < -89.0f)
							cam_pitch = -89.0f;

					ZSVECTOR3 front;
					front.X = (float)(cos(DegToRad(cam_yaw)) * cos(DegToRad(cam_pitch)));
					front.Y = -sin(DegToRad(cam_pitch));
					front.Z = sin(DegToRad(cam_yaw)) * cos(DegToRad(cam_pitch));
					vNormalize(&front);

					ZSpire::setCameraFront(front);
				}
			}

		}
		ImGui_ImplSdlGL3_NewFrame(window);

		if (IsSceneLoaded() == false) {
			ZSWindows::DrawStartupWindow(window);
		}
		else {
			ZSWindows::DrawObjectListWindow(window);
			ZSWindows::DrawInspectorWindow(window);
			ZSWindows::DrawFilesWindow(window);
			ZSWindows::DrawResourcesWindow(window);
		}

		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ZSpire::Renderer::RenderScene();

		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return 0;
}