#pragma once
#include <SDL.h>
#include <glad/glad.h>
#include <memory>

class Shader;
class VertexArray;
class VertexBuffer;
class ElementBuffer;
class Texture;

class Game {
private:
    // Private constructor to prevent outside instantiation
    Game();

private:
    SDL_GLContext glContext;
    SDL_Window* window;
    bool running = false;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<ElementBuffer> elementBuffer;
    std::unique_ptr<Texture> texture;
    GLuint indicesCount;

public:
    // Deleted copy constructor and assignment operator to prevent copying
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    // Static method to get the instance
    static Game* getInstance();

    bool initSDL();
    bool initGLAD();
    void setOpenGLAttributes();
    bool initOpenGL();
    bool initEverything();
    void initialize();

    bool isRunning();

    void processInput();
    void update();
    void render();

    void tutorial();

    void reset();
};

