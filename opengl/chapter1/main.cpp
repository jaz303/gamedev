#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <vector>

#include "glutil.hpp"

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

    printf("shader:\n%s\n\n", str);

    return str;
}

void initOpenGL(SDL_Renderer *renderer, GLuint program) {

     /* Enable smooth shading */
//    glShadeModel( GL_SMOOTH );
 
    /* Depth buffer setup */
    glClearDepth( 1.0f );
 
    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );
 
    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );
 
    /* Really Nice Perspective Calculations */
//    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glViewport(0, 0, 800, 600);

    const float vertexPositions[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f
    };

    GLuint positionBufferObject;

    glGenBuffers(1, &positionBufferObject);

    // binds the newly created buffer to the GL_ARRAY_BUFFER
    // target.
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

    // allocates memory for the buffer currently bound to GL_ARRAY_BUFFER
    // and copies the data from memory array to the buffer object.
    // can pass NULL for 3rd parameter to create buffer only without copying data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    // as far as OpenGL is concerned, this data is just meaningless
    // binary nonsense.

    // cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    // again, bind buffer to GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

    // first parameter is attribute index
    // (as defined in the shader)
    // this tells OpenGL to use a given attribute during
    // rendering
    glEnableVertexAttribArray(0);

    // this function implicitly addresses the buffer currently
    // bound to the GL_ARRAY_BUFFER target
    //
    // again, the first parameter is the attribute index
    //
    // this is us telling OpenGL what format the data in the
    // buffer object is stored in.
    //  - float data
    //  - each position has 4 of these
    //  - no space between each set of 4 values
    //  - first value is at the beginning of the object
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // first parameter says to take every 3 vertices as an
    // independent triangle.
    // second param => start at zeroth index
    // third param => read 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glUseProgram(0);

}

int main(int argc, char *argv[]) {

	SDL_Init(SDL_INIT_VIDEO);
	  
	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext glContext = SDL_GL_CreateContext(displayWindow);
    if (!glContext) {
        printf("Error creating OpenGL context!\n");
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));    

    SDL_GL_SetSwapInterval(1);

    GLuint theProgram = GLCreateDebugProgram();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    initOpenGL(displayRenderer, theProgram);
    SDL_GL_SwapWindow(displayWindow);

    int running = 1;
    while (running) {
        SDL_Event theEvent;
        while (SDL_PollEvent(&theEvent)) {
            if (theEvent.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(33);
    }
        
	SDL_Quit();
	   
	return 0;

}
