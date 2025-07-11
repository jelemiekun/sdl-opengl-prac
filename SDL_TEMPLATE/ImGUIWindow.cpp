#include <SDL.h>
#include <glad/glad.h>
#include "ImGuiWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
#include "ProgramValues.h"


ImGUIWindow::ImGUIWindow() {}

ImGUIWindow* ImGUIWindow::getInstance() {
	static ImGUIWindow instance;
	return &instance;
}

void ImGUIWindow::init(SDL_Window* window, SDL_GLContext* glContext) const {
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
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 430");
}

void ImGUIWindow::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();

	ImGui::ShowDemoWindow();

	{
		ImGui::Begin("Inputs");

		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse pos: ");
		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

		// We iterate both legacy native range and named ImGuiKey ranges. This is a little unusual/odd but this allows
		// displaying the data for old/new backends.
		// User code should never have to go through such hoops!
		// You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
		struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
		ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
		ImGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); }
		ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
		ImGui::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

		ImGui::End();
	}

	{
		ImGui::Begin("Photo Properties");

		{
			static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
			const ImGuiSliderFlags flags_for_sliders = flags & ~ImGuiSliderFlags_WrapAround;

			ImGui::SeparatorText("Scalar:");
			ImGui::Text("Scalar value: %.3f", ProgramValues::dimensionScalar);
			ImGui::SliderFloat("Scalar Value:", &ProgramValues::dimensionScalar, 0.0f, 10.0f, "%.3f");
		}


		{
			static ImGuiSliderFlags flags1 = ImGuiSliderFlags_None;
			const ImGuiSliderFlags flags_for_sliders1 = flags1 & ~ImGuiSliderFlags_WrapAround;

			ImGui::SeparatorText("Position:");
			ImGui::Text("X value: %.3f", ProgramValues::x);
			ImGui::SliderFloat("Slider X", &ProgramValues::x, -1.0f, 1.0f, "%.3f", flags_for_sliders1);

			ImGui::Text("Y value: %.3f", ProgramValues::y);
			ImGui::SliderFloat("Slider Y", &ProgramValues::y, -1.0f, 1.0f, "%.3f", flags_for_sliders1);
		}

		ImGui::End();
	}

	{
		ImGui::Begin("Color picker");

		static ImVec4 color1 = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
		static ImVec4 ref_color_v1(1.0f, 0.0f, 1.0f, 0.5f);
		static int display_mode1 = 0;
		static int picker_mode1 = 0;

		ImGui::SeparatorText("Color:");
		ImGuiColorEditFlags flags1 = ImGuiColorEditFlags_None;
		flags1 |= ImGuiColorEditFlags_AlphaBar;
		if (picker_mode1 == 1)  flags1 |= ImGuiColorEditFlags_PickerHueBar;
		if (picker_mode1 == 2)  flags1 |= ImGuiColorEditFlags_PickerHueWheel;
		if (display_mode1 == 1) flags1 |= ImGuiColorEditFlags_NoInputs;
		if (display_mode1 == 2) flags1 |= ImGuiColorEditFlags_DisplayRGB;
		if (display_mode1 == 3) flags1 |= ImGuiColorEditFlags_DisplayHSV;
		if (display_mode1 == 4) flags1 |= ImGuiColorEditFlags_DisplayHex;
		ImGui::ColorPicker4("MyColor##1", (float*)&color1, flags1, &ref_color_v1.x);

		ProgramValues::v1_color.r = color1.x;
		ProgramValues::v1_color.g = color1.y;
		ProgramValues::v1_color.b = color1.z;
		ProgramValues::v1_color.a = color1.w;

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

void ImGUIWindow::clean() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}