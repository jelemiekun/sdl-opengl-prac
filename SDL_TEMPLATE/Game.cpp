#include "Game.h"
#include <spdlog/spdlog.h>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

Game::Game() {}

 bool Game::initSDL() {
    spdlog::info("Initializing SDL...");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    }
    else {
        spdlog::info("Initializing sdl window...");

        window = SDL_CreateWindow("SDL OPEN_GL PRACTICE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
}

 void Game::tutorial() {

    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = std::make_unique<Shader>("source.shader");

    std::vector<GLfloat> vertices = {
        // Coordinates      Colors
        -0.2f,  0.2f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.2f,  0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.2f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.2f, -0.2f, 0.0f, 0.5f, 0.5f, 0.5f,
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        1, 2, 3
    };

    indicesCount = indices.size();

    vertexArray = std::make_unique<VertexArray>();
    vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));

    vertexArray->AddBuffer(*vertexBuffer, { 3, 3 });

    elementBuffer = std::make_unique<ElementBuffer>(indices.data(), indicesCount);
}

 void Game::update() {

}

 void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader->use();
    vertexArray->Bind();
    elementBuffer->Bind();
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    vertexArray->Unbind();

    SDL_GL_SwapWindow(window);
}

void Game::initialize() {
    spdlog::info("Initializing program...");

    if (initEverything()) {
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
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::isRunning() {
    return running;
}

Game* Game::getInstance() {
    static Game instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}