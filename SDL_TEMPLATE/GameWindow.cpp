#include "GameWindow.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Game.h"
#include "ImGuiWindow.h"
#include "ProgramValues.h"
#include "Model.h"

static Game* game = Game::getInstance();

static constexpr int INITIAL_WIDTH = 1280;
static constexpr int INITIAL_HEIGHT = 720;

static std::unique_ptr<Model> model1;
static std::unique_ptr<Model> model2;

static std::unique_ptr<Texture2D> textureObjectDiffuse;
static std::unique_ptr<Texture2D> textureObjectSpecular;
static std::unique_ptr<Camera> camera;

static int indicesCountObject;
static std::unique_ptr<Shader> shaderObject;
static std::unique_ptr<VertexArray> vaoObject;
static std::unique_ptr<VertexBuffer> vboObject;
static std::unique_ptr<ElementBuffer> eboObject;

static int indicesCountLight;
static std::unique_ptr<Shader> shaderLight;
static std::unique_ptr<VertexArray> vaoLight;
static std::unique_ptr<VertexBuffer> vboLight;
static std::unique_ptr<ElementBuffer> eboLight;

static glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

static glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

static glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)
};

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

    shaderObject = std::make_unique<Shader>("source.shader");

    std::vector<GLfloat> verticesObject = {
        // Position                Normal       Picture Coordinates
            -0.2f,  0.11f,  0.2f, 0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
             0.2f,  0.11f,  0.2f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
            -0.2f, -0.11f,  0.2f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
             0.2f, -0.11f,  0.2f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,

             -0.2f,  0.11f, -0.2f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
              0.2f,  0.11f, -0.2f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             -0.2f, -0.11f, -0.2f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
              0.2f, -0.11f, -0.2f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
              
              -0.2f,  0.11f, -0.2f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
              -0.2f,  0.11f,  0.2f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
              -0.2f, -0.11f, -0.2f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
              -0.2f, -0.11f,  0.2f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

               0.2f,  0.11f,  0.2f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
               0.2f,  0.11f, -0.2f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
               0.2f, -0.11f,  0.2f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
               0.2f, -0.11f, -0.2f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

               -0.2f,  0.11f, -0.2f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
                0.2f,  0.11f, -0.2f, 0.0f, 1.0f,  0.0f,  1.0f, 1.0f,
               -0.2f,  0.11f,  0.2f, 0.0f, 1.0f,  0.0f,  0.0f, 0.0f,
                0.2f,  0.11f,  0.2f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,

                -0.2f, -0.11f,  0.2f, 0.0f,  -1.0f,  0.0f,  0.0f, 1.0f,
                 0.2f, -0.11f,  0.2f, 0.0f,  -1.0f,  0.0f,  1.0f, 1.0f,
                -0.2f, -0.11f, -0.2f, 0.0f,  -1.0f,  0.0f,  0.0f, 0.0f,
                 0.2f, -0.11f, -0.2f, 0.0f,  -1.0f,  0.0f,  1.0f, 0.0f
    };

    std::vector<GLuint> indicesObject = {
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


    indicesCountObject = indicesObject.size();

    vaoObject = std::make_unique<VertexArray>();
    vboObject = std::make_unique<VertexBuffer>(verticesObject.data(), verticesObject.size() * sizeof(GLfloat));

    vaoObject->AddBuffer(*vboObject, { 3 , 3 , 2 });

    eboObject = std::make_unique<ElementBuffer>(indicesObject.data(), indicesCountObject);

    vaoObject->Bind();
    eboObject->Bind();
    vaoObject->Unbind();

    shaderLight = std::make_unique<Shader>("light.shader");

    std::vector<GLfloat> verticesLight = {
        // Position                Normal
            -0.2f,  0.2f,  0.2f,
             0.2f,  0.2f,  0.2f,
            -0.2f, -0.2f,  0.2f,
             0.2f, -0.2f,  0.2f,

             -0.2f,  0.2f, -0.2f,
              0.2f,  0.2f, -0.2f,
             -0.2f, -0.2f, -0.2f,
              0.2f, -0.2f, -0.2f
    };

    std::vector<GLuint> indicesLight = {
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7,
        0, 4, 1,
        4, 1, 5,
        2, 6, 3,
        6, 3, 7,
        0, 4, 2,
        4, 2, 6,
        1, 5, 3,
        5, 3, 7
    };

    indicesCountLight = indicesLight.size();

    vaoLight = std::make_unique<VertexArray>();
    vboLight = std::make_unique<VertexBuffer>(verticesLight.data(), verticesLight.size() * sizeof(GLfloat));
    
    vaoLight->AddBuffer(*vboLight, { 3 });

    eboLight = std::make_unique<ElementBuffer>(indicesLight.data(), indicesCountLight);

    vaoLight->Bind();
    eboLight->Bind();
    vboLight->Unbind();

    textureObjectDiffuse = std::make_unique<Texture2D>("assets/container2.png");
    textureObjectSpecular = std::make_unique<Texture2D>("assets/container2_specular.png");

    model1 = std::make_unique<Model>("assets/models/military_backpack/scene.gltf");
    model2 = std::make_unique<Model>("assets/models/american_high_school.glb");
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
    

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    shaderObject->setMat4("u_Model", model);
    model1->Draw(*shaderObject);
    
    
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
    shaderObject->setMat4("u_Model", model);
    model2->Draw(*shaderObject);
    
    glm::mat4 projection = glm::perspective(
        glm::radians(camera->getFOV()), 
        (float)mWidth / (float)mHeight,
        0.1f, 
        2000.0f
    );
    glm::mat4 objectModel = glm::mat4(1.0f);
    

    auto drawModel = [this, &projection, &objectModel](Shader& shader, glm::mat4& model, int indicesCount) -> void {
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        shaderObject->setMat4("u_Projection", projection);
        shaderObject->setMat4("u_View", camera->getViewMatrix());
        shader.setMat4("u_Model", model);
        shaderObject->setMat3("u_NormalMatrix", normalMatrix);

        shaderObject->setVec3("u_CameraPos", camera->position);

        shaderObject->setFloat("material.shininess", ProgramValues::Object::shininess);

        { // Directional Light
            shaderObject->setVec3("dirLight.direction", -glm::vec3(0.0f, 1.0f, 0.0f));
            shaderObject->setVec3("dirLight.ambient", glm::vec3(0.08f));
            shaderObject->setVec3("dirLight.diffuse", glm::vec3(0.3f));
            shaderObject->setVec3("dirLight.specular", glm::vec3(0.5f));
        }

        { // Spot Light
            shaderObject->setVec3("spotLight.position", camera->position);
            shaderObject->setVec3("spotLight.direction", camera->front);
            shaderObject->setFloat("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
            shaderObject->setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));
            shaderObject->setFloat("spotLight.constant", 1.0f);
            shaderObject->setFloat("spotLight.linear", 0.09f);
            shaderObject->setFloat("spotLight.quadratic", 0.032f);
            shaderObject->setVec3("spotLight.ambient", glm::vec3(0.0f));
            shaderObject->setVec3("spotLight.diffuse", glm::vec3(1.0f));
            shaderObject->setVec3("spotLight.specular", glm::vec3(1.0f));
        }

        { // Point Light
            for (int index = 0; index < 4; index++) {
                std::string name = "pointLight[" + std::to_string(index) + "].";

                shaderObject->setVec3(name + "position", pointLightPositions[index]);
                shaderObject->setFloat(name + "constant", 1.0f);
                shaderObject->setFloat(name + "linear", 0.09f);
                shaderObject->setFloat(name + "quadratic", 0.032f);
                shaderObject->setVec3(name + "ambient", glm::vec3(0.05f) * pointLightColors[index]);
                shaderObject->setVec3(name + "diffuse", glm::vec3(0.8f) * pointLightColors[index]);
                shaderObject->setVec3(name + "specular", glm::vec3(1.0f) * pointLightColors[index]);
            }
        }
        

        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    };

    { // Light
        shaderLight->use();
        vaoLight->Bind();

        for (int index = 0; index < 4; index++) {
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, pointLightPositions[index]);
            lightModel = glm::scale(lightModel, ProgramValues::LightSource::scale);
            lightModel = glm::rotate(lightModel, glm::radians(static_cast<float>(ProgramValues::LightSource::rotateDegrees)), ProgramValues::LightSource::rotate);


            // shaderLight->setVec3("u_ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            // shaderLight->setVec3("u_LightColor", glm::vec3(1.0f));
            shaderLight->setMat4("u_Projection", projection);
            shaderLight->setMat4("u_View", camera->getViewMatrix());
            shaderLight->setMat4("u_Model", lightModel);
            shaderLight->setVec3("u_LightColor", pointLightColors[index]);

            glDrawElements(GL_TRIANGLES, indicesCountLight, GL_UNSIGNED_INT, nullptr);
        }
    }

    { // Objects
        textureObjectDiffuse->bind(0);
        // shaderObject->setInt("material.diffuse", 0);
        shaderObject->use();
        vaoObject->Bind();

        for (unsigned int i = 0; i < 10; i++) {
            objectModel = glm::mat4(1.0f);
            objectModel = glm::scale(objectModel, glm::vec3(5.0f, 5.0f, 5.0f));
            objectModel = glm::translate(objectModel, cubePositions[i]);
            float angle = 20.0f * i;
            objectModel = glm::rotate(objectModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            drawModel(*shaderObject, objectModel, indicesCountObject);
        }

    }

    {
        textureObjectSpecular->bind(1);
        // shaderObject->setInt("material.specular", 1);
        shaderObject->use();
        vaoObject->Bind();

        objectModel = glm::mat4(1.0f);
        objectModel = glm::scale(objectModel, glm::vec3(5.0f, 5.0f, 5.0f));

        drawModel(*shaderObject, objectModel, indicesCountObject);
    }

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
