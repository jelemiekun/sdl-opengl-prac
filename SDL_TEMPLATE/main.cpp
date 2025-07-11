#include <SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

static SDL_GLContext glContext;
static SDL_Window* window;
static bool running = false;
static GLuint VAO;
static GLuint VBO;
static GLuint EBO;
static GLuint shaderProgram;
static GLuint indicesCount;

static bool initSDL() {
    spdlog::info("Initializing SDL...");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    } else {
        spdlog::info("Initializing sdl window...");

        window = SDL_CreateWindow("SDL OPEN_GL PRACTICE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

        if (!window) {
            spdlog::error("Window creation failed: {}", SDL_GetError());
            spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
            return false;
        } else {
            spdlog::info("Window initialized.");
        }

        spdlog::info("SDL initialized.");
        return true;
    }
}

static bool initGLAD() {
    spdlog::info("Initializing GLAD...");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
        return false;
    } else {
        spdlog::info("GLAD initialized.");
        return true;
    }
}

static void setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

static bool initOpenGL() {
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

static bool initEverything() {
    return initSDL() && initOpenGL() && initGLAD();
}

static void processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
}

static void tutorial() {
    const char* vertexShaderSource = "#version 430 core\n"
        "layout (location = 0) in vec3 aPos;\n"  // Input position attribute
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"  // Set the vertex position in clip space
        "}\0";

    // Define the fragment shader source code
    const char* fragmentShaderSource = "#version 430 core\n"
        "out vec4 FragColor;\n"  // Output color variable
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"  // Set fragment color
        "}\n\0";

    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint vertexSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (vertexSuccess) {
        spdlog::info("Vertex shader successfully compiled.");
    }
    else {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        spdlog::warn("Vertex shader failed to compile: {}", log);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLint fragmentSuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (fragmentSuccess) {
        spdlog::info("Fragment shader successfully compiled.");
    }
    else {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        spdlog::warn("Fragment shader failed to compile: {}", log);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    GLint linkingSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSuccess);
    if (linkingSuccess) {
        spdlog::info("Shader program linked successfully.");
    }
    else {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        spdlog::warn("Shader program failed to link: {}", log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<GLfloat> vertices = {
        -0.2f, 0.2f, 0.0f,
        0.2f, 0.2f, 0.0f,
        -0.2f, -0.2f, 0.0f,
        0.2f, -0.2f, 0.0f
    };

    std::vector<GLint> indices = {
        0, 1, 2,
        1, 2, 3
    };

    indicesCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

static void update() {
    
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glUseProgram(shaderProgram);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
}

int SDL_main(int argc, char* argv[]) {
    spdlog::info("Initializing program...");

    if (initEverything()) {
        spdlog::info("Program initialized.");

        running = true;

        tutorial();

        while (running) {
            processInput();
            update();
            render();
        }

        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteProgram(shaderProgram);

            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
    else {
        spdlog::error("Failed to initialize program.");
    }
    return 0;
}