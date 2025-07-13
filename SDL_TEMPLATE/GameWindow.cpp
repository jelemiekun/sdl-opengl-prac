#include "GameWindow.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>

static constexpr int INITIAL_WIDTH = 1280;
static constexpr int INITIAL_HEIGHT = 720;

// Constructor
GameWindow::GameWindow()
    : mWindow(nullptr), openGLContext(nullptr), mWindowID(-1),
    mWidth(0), mHeight(0), mMouseFocus(false), mKeyboardFocus(false),
    mFullScreen(false), mMinimized(false), mShown(false) {}

// Destructor
GameWindow::~GameWindow() {
    free();
}

// Initializes the window
bool GameWindow::init() {
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        INITIAL_WIDTH, INITIAL_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!mWindow) {
        spdlog::error("Window creation failed: {}", SDL_GetError());
        return false;
    }

    initVariables();

    spdlog::info("Window initialized successfully");
    return true;
}

// Handles events specific to this window
void GameWindow::handleEvent(SDL_Event& e) {
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
        switch (e.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                mShown = true;
                spdlog::info("Window {} shown", mWindowID);
                break;

            case SDL_WINDOWEVENT_HIDDEN:
                mShown = false;
                spdlog::info("Window {} hidden", mWindowID);
                break;

            case SDL_WINDOWEVENT_SIZE_CHANGED:
                mWidth = e.window.data1;
                mHeight = e.window.data2;
                glViewport(0, 0, mWidth, mHeight);
                spdlog::info("Window {} resized to {}x{}", mWindowID, mWidth, mHeight);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(mWindow);
                mShown = false;
                spdlog::info("Window {} closed (hidden)", mWindowID);
                break;
        }
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
        toggleFullscreen();
        spdlog::info("Toggled fullscreen mode");
    }
}

// Brings the window into focus
void GameWindow::focus() {
    if (!mShown) {
        SDL_ShowWindow(mWindow);
        spdlog::info("Window {} shown from focus()", mWindowID);
    }
    SDL_RaiseWindow(mWindow);
}

// Frees resources
void GameWindow::free() {
    if (mWindow) {
        spdlog::info("Destroying window {}", mWindowID);
        SDL_GL_DeleteContext(openGLContext);
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    mWidth = mHeight = 0;
}

void GameWindow::toggleFullscreen() {
    if (mFullScreen) {
        SDL_SetWindowFullscreen(mWindow, 0);  // Remove fullscreen
        mFullScreen = false;
        spdlog::info("Fullscreen disabled for window {}", mWindowID);
    } else {
        SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); // Set fullscreen
        mFullScreen = true;
        spdlog::info("Fullscreen enabled for window {}", mWindowID);
    }
}

void GameWindow::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(mWindow);
}

// Getters
int GameWindow::width() { return mWidth; }
int GameWindow::height() { return mHeight; }
bool GameWindow::hasMouseFocus() { return mMouseFocus; }
bool GameWindow::hasKeyboardFocus() { return mKeyboardFocus; }
bool GameWindow::isMinimized() { return mMinimized; }
bool GameWindow::isShown() { return mShown; }

SDL_Window* GameWindow::getWindow() {
    return mWindow ? mWindow : nullptr;
}

void GameWindow::initVariables() {
    mMouseFocus = mKeyboardFocus = true;
    mShown = true;
    mWidth = INITIAL_WIDTH;
    mHeight = INITIAL_HEIGHT;
    mWindowID = SDL_GetWindowID(mWindow);
}

void GameWindow::initOpenGlContextSetup() {
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

bool GameWindow::initOpenGLContext() {
    openGLContext = SDL_GL_CreateContext(mWindow);
    if (!openGLContext) {
        spdlog::error("SDL_GL_CreateContext failed: {}", SDL_GetError());
        return false;
    }

    if (SDL_GL_MakeCurrent(mWindow, openGLContext) < 0) {
        spdlog::error("SDL_GL_MakeCurrent failed: {}", SDL_GetError());
        return false;
    }

    spdlog::info("SDL_GL_CreateContext on Window {} initialized successfully.", mWindowID);
    return true;
}
