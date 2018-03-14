typedef unsigned int uint;

#include "imgui.h"
#include <SDL.h>
#include <vector>

#include "../includes/DMath.h"
#include "../includes/Camera.hpp"
#include "../includes/DColor.h"
#include "../includes/Light.h"
#include "../includes/property_inspector.h"
#include "../includes/GameObject.h"
#include "../includes/objects_window.h"
#include "../includes/scene_loader.h"

void DEWindows::DrawObjectListWindow(SDL_Window * window){
	bool show = true;
	ImGui::Begin("Game objects", &show, ImGuiWindowFlags_MenuBar); // ������ ����

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("New Object", "Ctrl+N")) { createNewObject(); }
			ImGui::EndMenu();
		}
		


		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { saveScene(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < getObjectsAmount(); n++) {
		if (ImGui::Button(getObject(n).label) == true) {
			DEWindows::Inspector::selectObject(n);
		}

	}

	ImGui::EndChild();

	

	ImGui::End(); // end window


}