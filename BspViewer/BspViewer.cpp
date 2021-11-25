#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <bitset>

#include "ViewerUtilities/Camera.h"

typedef std::bitset<2> viewer_params;
enum VIEW_PARAM : std::size_t { FULLSCREEN = 0, RUNNING = 1 };

int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0);

    uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    uint32_t WindowWidth = 1920;
    uint32_t WindowHeight = 1080;
    SDL_Window* Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, WindowFlags);
    //assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);

    /*GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };*/

    viewer_params params;
    params[VIEW_PARAM::FULLSCREEN] = 0;
    params[VIEW_PARAM::RUNNING] = 1;
    while (params[VIEW_PARAM::RUNNING])
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    params[VIEW_PARAM::RUNNING] = 0;
                    break;
                case 'f':
                    params[VIEW_PARAM::FULLSCREEN] = !params[VIEW_PARAM::FULLSCREEN];
                    WindowFlags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(Window, WindowFlags);
                    break;
                default:
                    break;
                }
            }
            else if (Event.type == SDL_QUIT)
            {
                params[VIEW_PARAM::RUNNING] = 0;
            }
        }

        glViewport(0, 0, WindowWidth, WindowHeight);
        glClearColor(0.07f, 0.05f, 0.02f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(Window);
    }
    return 0;
}
