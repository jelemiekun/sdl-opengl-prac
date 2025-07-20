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
	static ImGuiComboFlags flags = 0;

	ImGui::ShowDemoWindow();

	{
		ImGui::Begin("Object Properties");

		const char* items[] = { "pic.jpg", "container2.png"};
		static int item_selected_idx = 0; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = items[item_selected_idx];

		if (ImGui::BeginCombo("Object", combo_preview_value, flags)) {
			for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
				const bool is_selected = (item_selected_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_selected_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ProgramValues::Object* objectReference{};

		switch (item_selected_idx) {
			case 0: objectReference = ProgramValues::Objects::object0.get();  break;
			case 1: objectReference = ProgramValues::Objects::object1.get();  break;
			default: break;
		}

		if (objectReference) {
			ImGui::SeparatorText("Object Light Components Properties");
			ImGui::DragFloat("Ambient", &objectReference->ambient, 0.005f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("Diffuse", &objectReference->diffuse, 0.005f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("Specular", &objectReference->specular, 0.005f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("Shininess", &objectReference->shininess, 1.0f, 1.0f, 1024.0f, "%.1f", flags);



			ImGui::SeparatorText("Object Components Properties");

			ImGui::Text("Translate");
			ImGui::DragFloat("translate X", &objectReference->translate.x, 0.05f);
			ImGui::DragFloat("translate Y", &objectReference->translate.y, 0.05f);
			ImGui::DragFloat("translate Z", &objectReference->translate.z, 0.05f);

			ImGui::Text("Scale");
			ImGui::DragFloat("scale X", &objectReference->scale.x, 0.05f);
			ImGui::DragFloat("scale Y", &objectReference->scale.y, 0.05f);
			ImGui::DragFloat("scale Z", &objectReference->scale.z, 0.05f);

			ImGui::Text("Rotate");
			ImGui::DragFloat("Rotate Degrees", &objectReference->rotateDegrees, 0.05f, 0.0f, 360.0f, "%.3f", flags);
			ImGui::DragFloat("rotate X", &objectReference->rotate.x, 0.05f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("rotate Y", &objectReference->rotate.y, 0.05f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("rotate Z", &objectReference->rotate.z, 0.05f, 0.0f, 1.0f, "%.3f", flags);
		} else {
			ImGui::Text("No object selected.");
		}


		ImGui::End();
	}

	{
		ImGui::Begin("Light Properties");

		const char* items[] = { "light0" };
		static int item_selected_idx = 0; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = items[item_selected_idx];

		if (ImGui::BeginCombo("Light", combo_preview_value, flags)) {
			for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
				const bool is_selected = (item_selected_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_selected_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ProgramValues::Light* lightReference{};

		switch (item_selected_idx) {
			case 0: lightReference = ProgramValues::Lights::light0.get();  break;
			default: break;
		}

		if (lightReference) {
			ImGui::SeparatorText("Light Components Properties");
			ImGui::DragFloat("Ambient", &lightReference->ambient, 0.005f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("Diffuse", &lightReference->diffuse, 0.005f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("Specular", &lightReference->specular, 0.005f, 0.0f, 1.0f, "%.3f", flags);



			ImGui::SeparatorText("Light Properties");

			ImGui::Text("Translate");
			ImGui::DragFloat("translate X", &lightReference->translate.x, 0.05f);
			ImGui::DragFloat("translate Y", &lightReference->translate.y, 0.05f);
			ImGui::DragFloat("translate Z", &lightReference->translate.z, 0.05f);

			/*ImGui::Text("Scale");
			ImGui::DragFloat("scale X", &objectReference->scale.x, 0.05f);
			ImGui::DragFloat("scale Y", &objectReference->scale.y, 0.05f);
			ImGui::DragFloat("scale Z", &objectReference->scale.z, 0.05f);

			ImGui::Text("Rotate");
			ImGui::DragFloat("Rotate Degrees", &objectReference->rotateDegrees, 0.05f, 0.0f, 360.0f, "%.3f", flags);
			ImGui::DragFloat("rotate X", &objectReference->rotate.x, 0.05f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("rotate Y", &objectReference->rotate.y, 0.05f, 0.0f, 1.0f, "%.3f", flags);
			ImGui::DragFloat("rotate Z", &objectReference->rotate.z, 0.05f, 0.0f, 1.0f, "%.3f", flags);*/
		} else {
			ImGui::Text("No light selected.");
		}


		ImGui::End();
	}
	

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