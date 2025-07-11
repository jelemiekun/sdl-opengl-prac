#pragma once
#include <SDL.h>

class ImGUIWindow {
private:
    ImGUIWindow();

public:
    ImGUIWindow(const ImGUIWindow&) = delete;
    ImGUIWindow& operator=(const ImGUIWindow&) = delete;
    ImGUIWindow(ImGUIWindow&&) = delete;
    ImGUIWindow& operator=(ImGUIWindow&&) = delete;

    static ImGUIWindow* getInstance();

    void init(SDL_Window* window, SDL_GLContext* glContext) const;
    void render();
    void clean();
};