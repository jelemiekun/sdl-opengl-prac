#include "GameWindow.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <memory>

static constexpr int INITIAL_WIDTH = 1280;
static constexpr int INITIAL_HEIGHT = 720;

static int indicesCount;
static std::unique_ptr<Shader> shader;
static std::unique_ptr<Texture> texture;
static std::unique_ptr<VertexArray> vao;
static std::unique_ptr<VertexBuffer> vbo;
static std::unique_ptr<ElementBuffer> ebo;
static std::unique_ptr<Camera> camera;

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

void GameWindow::setupDraw() {
    camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f
    );

    shader = std::make_unique<Shader>("source.shader");

    std::vector<GLfloat> vertices = {
            -0.2f,  0.11f,  0.2f,   0.0f, 1.0f,
             0.2f,  0.11f,  0.2f,   1.0f, 1.0f,
            -0.2f, -0.11f,  0.2f,   0.0f, 0.0f,
             0.2f, -0.11f,  0.2f,   1.0f, 0.0f,

             -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
              0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
             -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
              0.2f, -0.11f, -0.2f,   1.0f, 0.0f,
              
              -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
              -0.2f,  0.11f,  0.2f,   1.0f, 1.0f,
              -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
              -0.2f, -0.11f,  0.2f,   1.0f, 0.0f,

               0.2f,  0.11f,  0.2f,   0.0f, 1.0f,
               0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
               0.2f, -0.11f,  0.2f,   0.0f, 0.0f,
               0.2f, -0.11f, -0.2f,   1.0f, 0.0f,

               -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
                0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
               -0.2f,  0.11f,  0.2f,   0.0f, 0.0f,
                0.2f,  0.11f,  0.2f,   1.0f, 0.0f,

                -0.2f, -0.11f,  0.2f,   0.0f, 1.0f,
                 0.2f, -0.11f,  0.2f,   1.0f, 1.0f,
                -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
                 0.2f, -0.11f, -0.2f,   1.0f, 0.0f
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7,
        8, 9, 10,
        9, 10, 11,
        12, 13, 14,
        13, 14, 15,
        16, 17, 18,
        17, 18, 19,
        20, 21, 22,
        21, 22, 23
    };


    indicesCount = indices.size();

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));

    vao->AddBuffer(*vbo, { 3, 2 });

    ebo = std::make_unique<ElementBuffer>(indices.data(), indicesCount);

    vao->Bind();
    ebo->Bind();
    vao->Unbind();

    shader->use();

    texture = std::make_unique<Texture>("assets/pic.jpg");
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

    camera->processKeyboard(e, this);
    camera->processMouseMotion(e);
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

void GameWindow::update() {
    camera->update();
}

void GameWindow::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader->use();

    {
        glm::mat4 projection = glm::perspective(
            glm::radians(camera->getFOV()), 
            (float)mWidth / (float)mHeight,
            0.1f, 
            100.0f
        );

        camera->setViewToShader(shader->ID, "u_View");

        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_Projection"), 1, GL_FALSE, &projection[0][0]);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(16.0f, 9.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_Model"), 1, GL_FALSE, &model[0][0]);
    }

    texture->bind(0); 
    shader->setInt("texture1", 0);
    vao->Bind();

    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);

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
