#include <SDL.h>

#include "../includes/zs-math.h"
#include "../includes/zs-camera.h"

#include "../includes/zs-app.h"

#include "../includes/zs-transform.h"

#include "../includes/zs-shader.h"

#include "../includes/zs-text-renderer.h"

#include "../includes/zs-input.h"

#include <windows.h>
#include <glew.h>

SDL_Window *window;
SDL_GLContext glcontext;

bool ZSpire::ZSpireApp::createWindow(ZSWindowDesc desc){

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		MessageBox(NULL, "Creating OpenGL 4.6 instance failed! ", TEXT("Error initializing OpenGL 4.6"), MB_OK);
		return false;
	}

	setCameraProjectionResolution((float)desc.WIDTH, (float)desc.HEIGHT);
	updateCameraMatrix();
	setLocalScreenSize(desc.WIDTH, desc.HEIGHT);

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, NULL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	int RESIZABLE_FLAG = SDL_WINDOW_RESIZABLE;
	if (desc.isResizable == false) RESIZABLE_FLAG = 0;


	window = SDL_CreateWindow(desc.WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desc.WIDTH, desc.HEIGHT, SDL_WINDOW_OPENGL | RESIZABLE_FLAG);
	glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval((int)desc.isVsyncEnabled); // Enable vsync

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, "Creating OpenGL 4.6 instance failed! ", TEXT("Error initializing OpenGL 4.6"), MB_OK);

		return false;
	}

	glEnable(GL_DEPTH_TEST);

	return true;
}

void ZSpire::ZSpireApp::ZSDestroyWindow() {
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void ZSpire::ZSpireApp::MSGBox(const char* title, const char* message) {
	MessageBox(NULL, message, title, MB_OK);

}

void ZSpire::ZSpireApp::PollEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {



	case SDL_KEYDOWN: {
		int cc = event.key.keysym.sym;
		addKeyToQueue(cc);
		break;
	}

	case SDL_MOUSEMOTION: {
		setMouseStateXYPOSvalue(event.motion.x, event.motion.y);
		//OnMouseMove(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		break;
	}

	case SDL_MOUSEBUTTONDOWN: {
		switch (event.button.button) {
		case SDL_BUTTON_LEFT: {
			setMouseStateLeftButtonDownBool(true);
			break;
		}
		case SDL_BUTTON_RIGHT: {
			setMouseStateRightButtonDownBool(true);
			break;
		}
		case SDL_BUTTON_MIDDLE: {
			setMouseStateWheelButtonDownBool(true);
			break;
		}
		}
		break;
	}


	}
}

void ZSpire::ZSpireApp::postFrame() {

	SDL_GL_SwapWindow(window);
}

void ZSpire::ZSpireApp:: gl_clear() {
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
