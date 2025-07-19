#include <SDL.h>
#include <glad/glad.h>
#include <string>
#include <spdlog/spdlog.h>
#include "ImGuiWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "ProgramValues.h"

const static constexpr char* OPENGL_VERSION = "#version 430";

ImGuiWindow::ImGuiWindow() {}

ImGuiWindow* ImGuiWindow::getInstance() {
	static ImGuiWindow instance;
	return &instance;
}

void ImGuiWindow::init(SDL_Window* window, SDL_GLContext glContext) const {
	spdlog::info("ImGui initializing...");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	if (!ImGui_ImplSDL2_InitForOpenGL(window, glContext)) {
		spdlog::error("Failed to initialize ImGui SDL2 backend.");
		return;
	}
	
	if (!ImGui_ImplOpenGL3_Init(OPENGL_VERSION)) {
		spdlog::error("Failed to initialize ImGui OpenGL3 backend.");
		return;
	}

	spdlog::info("ImGui initialized successfully.");
}

void ImGuiWindow::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

	ImGui::ShowDemoWindow();

	ImGui::Begin("Light Source");
	ImGui::SeparatorText("Properties");

	const char* lightWindowChildLabels[] = {"Ambient", "Diffuse", "Specular", "Translate", "Scale", "Rotate" };
	const int lightWindowNumLabels = sizeof(lightWindowChildLabels) / sizeof(lightWindowChildLabels[0]);
	const float LIGHT_SCALAR = 0.005f;

	for (int i = 0; i < lightWindowNumLabels; i++) {
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		ImGui::PushID(i);
		if (ImGui::TreeNode("", "%s", lightWindowChildLabels[i])) {
			if (i == 0) {
				ImGui::DragFloat("X value", &ProgramValues::LightSource::ambient.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::ambient.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::ambient.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::LightSource::ambient.x < 0.1f) ProgramValues::LightSource::ambient.x = 0.1f;
				if (ProgramValues::LightSource::ambient.y < 0.1f) ProgramValues::LightSource::ambient.y = 0.1f;
				if (ProgramValues::LightSource::ambient.z < 0.1f) ProgramValues::LightSource::ambient.z = 0.1f;
			}
			if (i == 1) {
				ImGui::DragFloat("X value", &ProgramValues::LightSource::diffuse.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::diffuse.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::diffuse.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::LightSource::diffuse.x < 0.1f) ProgramValues::LightSource::diffuse.x = 0.1f;
				if (ProgramValues::LightSource::diffuse.y < 0.1f) ProgramValues::LightSource::diffuse.y = 0.1f;
				if (ProgramValues::LightSource::diffuse.z < 0.1f) ProgramValues::LightSource::diffuse.z = 0.1f;
			}
			if (i == 2) {
				ImGui::DragFloat("X value", &ProgramValues::LightSource::specular.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::specular.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::specular.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::LightSource::specular.x < 0.1f) ProgramValues::LightSource::specular.x = 0.1f;
				if (ProgramValues::LightSource::specular.y < 0.1f) ProgramValues::LightSource::specular.y = 0.1f;
				if (ProgramValues::LightSource::specular.z < 0.1f) ProgramValues::LightSource::specular.z = 0.1f;
			}
			if (i == 3) {
				ImGui::DragFloat("X value", &ProgramValues::LightSource::position.x, LIGHT_SCALAR);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::position.y, LIGHT_SCALAR);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::position.z, LIGHT_SCALAR);
			}
			if (i == 4) {
				ImGui::DragFloat("X value", &ProgramValues::LightSource::scale.x, LIGHT_SCALAR);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::scale.y, LIGHT_SCALAR);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::scale.z, LIGHT_SCALAR);

				if (ProgramValues::LightSource::scale.x < 0.1f) ProgramValues::LightSource::scale.x = 0.1f;
				if (ProgramValues::LightSource::scale.y < 0.1f) ProgramValues::LightSource::scale.y = 0.1f;
				if (ProgramValues::LightSource::scale.z < 0.1f) ProgramValues::LightSource::scale.z = 0.1f;
			}
			if (i == 5) {
				ImGui::DragInt("Degrees: ", &ProgramValues::LightSource::rotateDegrees, 1.0f, 1, 360, "%d", flags);
				ImGui::DragFloat("X value", &ProgramValues::LightSource::rotate.x, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::LightSource::rotate.y, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::LightSource::rotate.z, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);

				if (ProgramValues::LightSource::rotate.x < 1.0f) ProgramValues::LightSource::rotate.x = 1.0f;
				if (ProgramValues::LightSource::rotate.y < 1.0f) ProgramValues::LightSource::rotate.y = 1.0f;
				if (ProgramValues::LightSource::rotate.z < 1.0f) ProgramValues::LightSource::rotate.z = 1.0f;
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	ImGui::End();

	ImGui::Begin("Objects");
	const char* objectWindowChildLabels[] = {"Object Count", "Spacing", "Revolve", "Shininess", "Ambient", "Diffuse", "Specular", "Scale", "Rotate" };
	const int objectWindowNumLabels = sizeof(objectWindowChildLabels) / sizeof(objectWindowChildLabels[0]);

	for (int i = 0; i < objectWindowNumLabels; i++) {
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		ImGui::PushID(i);
		if (ImGui::TreeNode("%s", objectWindowChildLabels[i])) {
			if (i == 0) {
				ImGui::DragInt("Count", &ProgramValues::Object::count, 1, 1, 256, "%d", flags);
			}
			if (i == 1) {
				ImGui::DragFloat("Spacing", &ProgramValues::Object::spacing, 0.25f, 0.1f, 1000.0f, "%.3f", flags);
			}
			if (i == 2) {
				ImGui::Checkbox("Revolve", &ProgramValues::Object::revolve);

				if (!ProgramValues::Object::revolve)
					ImGui::BeginDisabled();

				ImGui::DragFloat("Speed", &ProgramValues::Object::revolveSpeed, 0.00005f, 0.00005f, 1.0f, "%.5f", flags);

				if (!ProgramValues::Object::revolve)
					ImGui::EndDisabled();
			}
			if (i == 3) {
				ImGui::DragInt("Shininess", &ProgramValues::Object::shininess, LIGHT_SCALAR, 1, 256, "%d", flags);
			}
			if (i == 4) {
				ImGui::DragFloat("X value", &ProgramValues::Object::ambient.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::Object::ambient.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::Object::ambient.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::Object::ambient.x < 0.1f) ProgramValues::Object::ambient.x = 0.1f;
				if (ProgramValues::Object::ambient.y < 0.1f) ProgramValues::Object::ambient.y = 0.1f;
				if (ProgramValues::Object::ambient.z < 0.1f) ProgramValues::Object::ambient.z = 0.1f;
			}
			if (i == 5) {
				ImGui::DragFloat("X value", &ProgramValues::Object::diffuse.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::Object::diffuse.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::Object::diffuse.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::Object::diffuse.x < 0.1f) ProgramValues::Object::diffuse.x = 0.1f;
				if (ProgramValues::Object::diffuse.y < 0.1f) ProgramValues::Object::diffuse.y = 0.1f;
				if (ProgramValues::Object::diffuse.z < 0.1f) ProgramValues::Object::diffuse.z = 0.1f;
			}
			if (i == 6) {
				ImGui::DragFloat("X value", &ProgramValues::Object::specular.x, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::Object::specular.y, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::Object::specular.z, LIGHT_SCALAR, 0.1f, 1.0f, "%.3f", flags);

				if (ProgramValues::Object::specular.x < 0.1f) ProgramValues::Object::specular.x = 0.1f;
				if (ProgramValues::Object::specular.y < 0.1f) ProgramValues::Object::specular.y = 0.1f;
				if (ProgramValues::Object::specular.z < 0.1f) ProgramValues::Object::specular.z = 0.1f;
			}
			if (i == 7) {
				ImGui::DragFloat("X value", &ProgramValues::Object::scale.x, LIGHT_SCALAR);
				ImGui::DragFloat("Y value", &ProgramValues::Object::scale.y, LIGHT_SCALAR);
				ImGui::DragFloat("Z value", &ProgramValues::Object::scale.z, LIGHT_SCALAR);

				if (ProgramValues::Object::scale.x < 0.1f) ProgramValues::Object::scale.x = 0.1f;
				if (ProgramValues::Object::scale.y < 0.1f) ProgramValues::Object::scale.y = 0.1f;
				if (ProgramValues::Object::scale.z < 0.1f) ProgramValues::Object::scale.z = 0.1f;
			}
			if (i == 8) {
				ImGui::Checkbox("Auto Rotate", &ProgramValues::Object::autoRotate);

				if (ProgramValues::Object::autoRotate)
					ImGui::DragFloat("Speed: ", &ProgramValues::Object::autoRotateSpeed, 0.0005f, 0.0005f, 1.0f, "%.4f", flags);

				if (ProgramValues::Object::autoRotate)
					ImGui::BeginDisabled();

				ImGui::DragFloat("Degrees: ", &ProgramValues::Object::rotateDegrees, 1.0f, 1.0f, 360.0f, "%.4f", flags);
				ImGui::DragFloat("X value", &ProgramValues::Object::rotate.x, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);
				ImGui::DragFloat("Y value", &ProgramValues::Object::rotate.y, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);
				ImGui::DragFloat("Z value", &ProgramValues::Object::rotate.z, LIGHT_SCALAR, 1.0f, 360.0f, "%.3f", flags);

				if (ProgramValues::Object::rotate.x < 1.0f) ProgramValues::Object::rotate.x = 1.0f;
				if (ProgramValues::Object::rotate.y < 1.0f) ProgramValues::Object::rotate.y = 1.0f;
				if (ProgramValues::Object::rotate.z < 1.0f) ProgramValues::Object::rotate.z = 1.0f;

				if (ProgramValues::Object::autoRotate)
					ImGui::EndDisabled();
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void ImGuiWindow::clean() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}