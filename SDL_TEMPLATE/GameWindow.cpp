#include "GameWindow.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Game.h"
#include "ImGuiWindow.h"
#include "ProgramValues.h"

static Game* game = Game::getInstance();

static constexpr int INITIAL_WIDTH = 1280;
static constexpr int INITIAL_HEIGHT = 720;

static std::unique_ptr<Camera> camera;

static std::unique_ptr<Shader> shaderObject;
static std::unique_ptr<VertexArray> vaoObject;
static std::unique_ptr<VertexBuffer> vboObject;
static std::unique_ptr<ElementBuffer> eboObject;
static std::unique_ptr<Texture> texture1;
static std::unique_ptr<Texture> texture2DiffuseMap;
static std::unique_ptr<Texture> texture2SpecularMap;
static int objectIndicesCount;

static std::unique_ptr<Shader> shaderLight;
static std::unique_ptr<VertexArray> vaoLight;
static std::unique_ptr<VertexBuffer> vboLight;
static std::unique_ptr<ElementBuffer> eboLight;
static std::unique_ptr<Texture> texture1Light;

static ProgramValues::Object* objectReference;

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

    texture1 = std::make_unique<Texture>("assets/pic.jpg");

    shaderObject = std::make_unique<Shader>("source.shader");

    std::vector<GLfloat> objectVertices = {
        // Position,         Image Coords    Norm
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,

         -1.0f,  1.0f, -1.0f, 1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
          1.0f,  1.0f, -1.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
         -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
          1.0f, -1.0f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,

          -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
          -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
          -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

           1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
           1.0f,  1.0f, -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

          -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
          -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
           1.0f,  1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

          -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
          -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
           1.0f, -1.0f, -1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f
    };

    std::vector<GLuint> objectIndices = {
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

    objectIndicesCount = objectIndices.size();

    vaoObject = std::make_unique<VertexArray>();
    vboObject = std::make_unique<VertexBuffer>(objectVertices.data(), objectVertices.size() * sizeof(GLfloat));

    vaoObject->AddBuffer(*vboObject, { 3 , 2 , 3 });

    eboObject = std::make_unique<ElementBuffer>(objectIndices.data(), objectIndicesCount);

    vaoObject->Bind();
    eboObject->Bind();


    texture2DiffuseMap = std::make_unique<Texture>("assets/container2.png");
    texture2SpecularMap = std::make_unique<Texture>("assets/container2_specular.png");


    shaderLight = std::make_unique<Shader>("light.shader");
    vaoLight = std::make_unique<VertexArray>();
    vboLight = std::make_unique<VertexBuffer>(objectVertices.data(), objectVertices.size() * sizeof(GLfloat));

    vaoLight->AddBuffer(*vboLight, { 3 , 2 , 3 });

    eboLight = std::make_unique<ElementBuffer>(objectIndices.data(), objectIndicesCount);

    vaoLight->Bind();
    eboLight->Bind();

    texture1Light = std::make_unique<Texture>("assets/On_Redstone_Lamp.png");
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

            case SDL_WINDOWEVENT_RESTORED:
                ProgramValues::KeyEvents::isLockedIn = true;
                spdlog::info("Window {} restored", mWindowID);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                game->setRunning(false);
                SDL_HideWindow(mWindow);
                mShown = false;
                spdlog::info("Window {} closed and program stopping", mWindowID);
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

    glm::mat4 projection = glm::perspective(
        glm::radians(camera->fov),
        static_cast<float>(mWidth) / static_cast<float>(mHeight),
        0.1f,
        100.0f
    );
    glm::mat4 view = camera->getViewMatrix();

    // Lambda to draw a generic object
    auto drawObject = [this, &view, &projection](const std::unique_ptr<ProgramValues::Object>& obj, const std::unique_ptr<Texture>& texture, int textureSlot) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj->translate);
        model = glm::scale(model, obj->scale);
        model = glm::rotate(model, glm::radians(obj->rotateDegrees), obj->rotate);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        shaderObject->bind();
        vaoObject->Bind();
        texture->bind(textureSlot);

        switch (textureSlot) {
            case 0: shaderObject->setInt("material.diffuse", textureSlot); break;
            case 1: shaderObject->setInt("material.diffuse", textureSlot); break;
            case 2: shaderObject->setInt("material.specular", textureSlot); break;
            default: break;
        }

        shaderObject->setMat3("u_NormalMatrix", normalMatrix);
        shaderObject->setMat4("u_Model", model);
        shaderObject->setMat4("u_View", view);
        shaderObject->setMat4("u_Projection", projection);
        shaderObject->setVec3("u_CameraPos", camera->position);

        // shaderObject->setVec3("material.ambientStrength", glm::vec3(obj->ambient));
        // shaderObject->setVec3("material.diffuseStrength", glm::vec3(obj->diffuse));
        // shaderObject->setVec3("material.specularStrength", glm::vec3(obj->specular));
        shaderObject->setFloat("material.shininess", obj->shininess);

        shaderObject->setVec3("light.ambient", glm::vec3(ProgramValues::Lights::light0->ambient));
        shaderObject->setVec3("light.diffuse", glm::vec3(ProgramValues::Lights::light0->diffuse));
        shaderObject->setVec3("light.specular", glm::vec3(ProgramValues::Lights::light0->specular));
        shaderObject->setVec3("light.position", ProgramValues::Lights::light0->translate);

        glDrawElements(GL_TRIANGLES, objectIndicesCount, GL_UNSIGNED_INT, nullptr);
        };

    // Lambda to draw the light as an object
    auto drawLight = [this, &projection, &view]() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ProgramValues::Lights::light0->translate);

        shaderLight->bind();
        vaoLight->Bind();
        texture1Light->bind(0);
        shaderLight->setInt("texture1", 0);

        shaderLight->setMat4("u_Projection", projection);
        shaderLight->setMat4("u_View", view);
        shaderLight->setMat4("u_Model", model);

        glDrawElements(GL_TRIANGLES, objectIndicesCount, GL_UNSIGNED_INT, nullptr);
        };

    // Draw objects
    drawObject(ProgramValues::Objects::object0, texture1, 0);
    drawObject(ProgramValues::Objects::object1, texture2DiffuseMap, 1);
    drawObject(ProgramValues::Objects::object1, texture2SpecularMap, 2);

    // Draw light
    drawLight();

    // Render ImGui
    game->imGuiWindow->render();
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

SDL_GLContext& GameWindow::getGLContext() {
    return openGLContext;
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
