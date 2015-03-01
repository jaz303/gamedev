#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <vector>
#include <cmath>

#include "glutil.hpp"

static int windowWidth;
static int windowHeight;
static SDL_GLContext glContext;

char *loadFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *str = (char*)malloc(size + 1);
    fread(str, size, 1, file);
    fclose(file);

    str[size] = 0;

    return str;
}

void resize()
{
    glViewport(0, 0, windowWidth, windowHeight);
}

void initOpenGL(SDL_Window *window)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Error creating OpenGL context!\n");
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));    

    SDL_GL_SetSwapInterval(1);
    
    //glClearDepth(1.0f);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    resize();
}

//
//

static const float vertexData[] = {
    // positions
    -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.0f, 1.0f,
    // colors
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

GLuint program;
GLuint vertexBufferObject;

void init()
{
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // GL_STREAM_DRAW indicates that this buffer data will be updated
    // GL_STATIC_DRAW means buffer data will only be set once
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    program = GLCreateProgram(
        loadFile("vertex-shader.glsl"),
        loadFile("fragment-shader.glsl"));
}

void draw(int dt)
{
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    
    GLuint offsetLocation = glGetUniformLocation(program, "loopDuration");
    // 1f - takes one float value
    // it isnt' strictly necessary to set this once per loop
    // because all objects that maintain internal state will retain that
    // state unless you explicitly change it.
    glUniform1f(offsetLocation, 5.0f);

    offsetLocation = glGetUniformLocation(program, "time");
    glUniform1f(offsetLocation, SDL_GetTicks() / 1000.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);
}

int main(int argc, char *argv[]) {

	SDL_Init(SDL_INIT_VIDEO);
	  
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;

    windowWidth = 800;
    windowHeight = 600;
   	SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);

    initOpenGL(displayWindow);
    init();


    int lastTick = SDL_GetTicks();
    SDL_Delay(1000 / 60);
    
    int running = 1;
    while (running) {
        SDL_Event theEvent;
        while (SDL_PollEvent(&theEvent)) {
            if (theEvent.type == SDL_QUIT) {
                running = 0;
            }
        }
        int now = SDL_GetTicks();
        draw(now - lastTick);
        lastTick = now;
        SDL_GL_SwapWindow(displayWindow);
        SDL_Delay(1000 / 60);
    }
        
	SDL_Quit();
	   
	return 0;

}
