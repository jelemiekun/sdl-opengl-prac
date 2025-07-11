#pragma once
#include <SDL.h>

class HandleEvent {
private:
    // Private constructor to prevent outside instantiation
    HandleEvent();

public:
    // Deleted copy constructor and assignment operator to prevent copying
    HandleEvent(const HandleEvent&) = delete;
    HandleEvent& operator=(const HandleEvent&) = delete;
    HandleEvent(HandleEvent&&) = delete;
    HandleEvent& operator=(HandleEvent&&) = delete;

    // Static method to get the instance
    static HandleEvent* getInstance();

    void input(SDL_Event& event);
};

