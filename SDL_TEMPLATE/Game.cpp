#include "Game.h"
#include <spdlog/spdlog.h>
#include "HandleEvent.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include <imgui_impl_sdl2.h>
#include "ImGUIWindow.h"
#include "ProgramValues.h"

Game::Game() {}

 bool Game::initSDL() {
    spdlog::info("Initializing SDL...");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    }
    else {
        spdlog::info("Initializing sdl window...");

        window = SDL_CreateWindow(
            "SDL OPEN_GL PRACTICE", 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            ProgramValues::ProgramDimensionX, 
            ProgramValues::ProgramDimensionY, 
            SDL_WINDOW_OPENGL);

        if (!window) {
            spdlog::error("Window creation failed: {}", SDL_GetError());
            spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
            return false;
        }
        else {
            spdlog::info("Window initialized.");
        }

        spdlog::info("SDL initialized.");
        return true;
    }
}

 bool Game::initGLAD() {
    spdlog::info("Initializing GLAD...");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
        return false;
    }
    else {
        spdlog::info("GLAD initialized.");
        return true;
    }
}

 void Game::initImGUI() {
     imGUIWindow = ImGUIWindow::getInstance();
     imGUIWindow->init(window, &glContext);
 }

 void Game::initHandleEvent() {
     spdlog::info("Initializing event handler...");
     handleEvent = HandleEvent::getInstance();
     spdlog::info("Event handler initialized successfully.");
 }

 void Game::setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

 bool Game::initOpenGL() {
    spdlog::info("Initializing OpenGL...");

    setOpenGLAttributes();

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        return false;
        spdlog::error("SDL_GL_CreateContext failed: {}", SDL_GetError());
    }

    spdlog::info("OpenGL initialized.");
    return true;
}

 bool Game::initEverything() {
    return initSDL() && initOpenGL() && initGLAD();
}

 void Game::processInput() {
     handleEvent->input(event);
}

 void Game::tutorial() {

    glViewport(0, 0, ProgramValues::ProgramDimensionX, ProgramValues::ProgramDimensionY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = std::make_unique<Shader>("source.shader");

    { // Colors
        std::vector<GLfloat> vertices = {
            // Coords               Picture Coords      Colors
             -0.9f, 0.6f, 0.0f,     0.0f, 1.0f,         
              0.9f, 0.6f, 0.0f,     1.0f, 1.0f,  
             -0.9f,-0.6f, 0.0f,     0.0f, 0.0f,       
              0.9f,-0.6f, 0.0f,     1.0f, 0.0f
        };

        std::vector<GLuint> indices = {
            0, 1, 2,
            1, 2, 3
        };

        indicesCount = indices.size();

        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));

        vertexArray->AddBuffer(*vertexBuffer, { 3 , 2 });

        elementBuffer = std::make_unique<ElementBuffer>(indices.data(), indicesCount);
    }


    shader->use();

    texture = std::make_unique<Texture>("assets/pic.jpg");
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
    glUniform3f(glGetUniformLocation(shader->ID, "u_ModifiedCoords"), ProgramValues::x, ProgramValues::x, 0.8f);
}

 void Game::update() {
     {
         glUniform3f(glGetUniformLocation(shader->ID, "u_ModifiedCoords"), ProgramValues::x, ProgramValues::y, 0.0f);

         glUniform4f(
             glGetUniformLocation(shader->ID, "u_Color"),
             ProgramValues::v1_color.r,
             ProgramValues::v1_color.g,
             ProgramValues::v1_color.b,
             ProgramValues::v1_color.a
         );

         glUniform1f(
             glGetUniformLocation(shader->ID, "u_DimensionScalar"),
             ProgramValues::dimensionScalar
         );
     }

     texture->bind();
}

 void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader->use();
    vertexArray->Bind();
    elementBuffer->Bind();
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    vertexArray->Unbind();

    imGUIWindow->render();

    SDL_GL_SwapWindow(window);
}

void Game::initialize() {
    spdlog::info("Initializing program...");

    if (initEverything()) {
        initImGUI();
        initHandleEvent();

        spdlog::info("Program initialized.");
        running = true;
    }
    else {
        spdlog::error("Failed to initialize program.");
    }
}

void Game::reset() {
    vertexArray.reset();
    vertexBuffer.reset();
    elementBuffer.reset();
    shader.reset();
    imGUIWindow->clean();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::isRunning() {
    return running;
}

void Game::setRunning(bool running) {
    this->running = running;
}

Game* Game::getInstance() {
    static Game instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}