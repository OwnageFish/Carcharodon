#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>
#include <iostream>
#include <bitset>

#include "ViewerUtilities/Camera.h"
#include "ViewerUtilities/Shader.h"
#include "ViewerUtilities/Framebuffer.h"
#include "ViewerUtilities/RefGrid.h"

float deltaTickTime;
int curTicks, prevTicks;

typedef std::bitset<3> viewer_params;
enum VIEW_PARAM : std::size_t { FULLSCREEN = 0, RUNNING = 1, MOUSELOOK = 2 };

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::string src, tp, sev;

    switch (source) {
    case GL_DEBUG_SOURCE_API:				src = "API";				break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		src = "WINDOW SYSTEM";		break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:	src = "SHADER COMPILER";	break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:		src = "THIRD PARTY";		break;
    case GL_DEBUG_SOURCE_APPLICATION:		src = "APPLICATION";		break;
    case GL_DEBUG_SOURCE_OTHER:				src = "OTHER";				break;
    default:								src = "UNKNOWN";			break;
    }
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:				tp = "ERROR";				break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	tp = "DEPRECATED BEHAVIOR";	break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	tp = "UDEFINED BEHAVIOR";	break;
    case GL_DEBUG_TYPE_PORTABILITY:			tp = "PORTABILITY";			break;
    case GL_DEBUG_TYPE_PERFORMANCE:			tp = "PERFORMANCE";			break;
    case GL_DEBUG_TYPE_OTHER:				tp = "OTHER";				break;
    case GL_DEBUG_TYPE_MARKER:				tp = "MARKER";				break;
    default:								tp = "UNKNOWN";				break;
    }
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:			sev = "HIGH";			break;
    case GL_DEBUG_SEVERITY_MEDIUM:			sev = "MEDIUM";			break;
    case GL_DEBUG_SEVERITY_LOW:				sev = "LOW";			break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:	sev = "NOTIFICATION";	break;
    default:								sev = "UNKNOWN";		break;
    }

    fprintf(stderr, "%d: %s of %s severity, raised from %s: %s\n", id, tp.c_str(), sev.c_str(), src.c_str(), message);
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

    uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    GLint WindowWidth = 1920; GLint WindowHeight = 1080;
    SDL_Window* Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, WindowFlags);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
    glViewport(0, 0, WindowWidth, WindowHeight);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
        return EXIT_FAILURE;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, NULL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera::Object cam(glm::vec3(0.0f, 1.0f, 0.0f));
    Camera::MoveInputs mv;

    ScreenFramebuffer scrRenderTarget(1, WindowWidth, WindowHeight);
    Shader frameBufferShader("Shaders/fb_vert.glsl", "Shaders/fb_frag.glsl");
    frameBufferShader.use();
    frameBufferShader.setInt("screenTexture", 0);

    GridInternal g(2.5, 5);

    GLuint spaceTransUniform;
    struct spaceTransforms {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    } spaces;

    spaces.model = glm::scale(glm::mat4(1.0), glm::vec3(0.01));
    spaces.view = cam.GetViewMatrix();
    spaces.projection = glm::perspective(glm::radians(cam.look_opts.field_of_view), static_cast <float> (WindowWidth) / WindowHeight, cam.look_opts.near_z_plane, cam.look_opts.far_z_plane);

    // Set binding points
    GLuint spaceTfBindPt = 0;
    glUniformBlockBinding(g.shade->ID, glGetUniformBlockIndex(g.shade->ID, "spaceTransforms"), spaceTfBindPt);

    // Generate transformation buffer and put initial matrices into that buffer
    glGenBuffers(1, &spaceTransUniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, spaceTfBindPt, spaceTransUniform); // Bind UBO to binding point as specified above (we now use a binding point)
    glBufferData(GL_UNIFORM_BUFFER, sizeof(spaces), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(spaces.model));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(spaces.view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(spaces.projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);



    /*GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    GLuint vertexbuffer; // This will identify our vertex buffer
    glGenBuffers(1, &vertexbuffer); // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // The following commands will talk about our 'vertexbuffer' buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // Give our vertices to OpenGL.

    Shader basic_shader ("Shaders/basic.vert.glsl", "Shaders/basic.frag.glsl");*/

    prevTicks = SDL_GetTicks();

    viewer_params params;
    params[VIEW_PARAM::FULLSCREEN] = 0;
    params[VIEW_PARAM::RUNNING] = 1;
    params[VIEW_PARAM::MOUSELOOK] = 0;
    while (params[VIEW_PARAM::RUNNING])
    {
        curTicks = SDL_GetTicks();
        deltaTickTime = ((float)curTicks - prevTicks) / 1000.0f;
        prevTicks = curTicks;

        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN) {

                switch (Event.key.keysym.sym)
                {
                case 'f':
                    params[VIEW_PARAM::FULLSCREEN] = !params[VIEW_PARAM::FULLSCREEN];
                    WindowFlags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(Window, WindowFlags);
                    break;
                case SDLK_z:
                    params[VIEW_PARAM::MOUSELOOK] = !params[VIEW_PARAM::MOUSELOOK];
                    if (params[VIEW_PARAM::MOUSELOOK])  SDL_ShowCursor(SDL_DISABLE);
                    else                                SDL_ShowCursor(SDL_ENABLE);
                    break;
                case SDLK_ESCAPE:
                case SDLK_q:        params[VIEW_PARAM::RUNNING] = 0;    break;
                case SDLK_w:        mv.moveKeys.set(Camera::FWRD);      break;
                case SDLK_s:        mv.moveKeys.set(Camera::BACK);      break;
                case SDLK_a:        mv.moveKeys.set(Camera::LEFT);      break;
                case SDLK_d:        mv.moveKeys.set(Camera::RGHT);      break;
                case SDLK_SPACE:    mv.moveKeys.set(Camera::SPCE);      break;
                case SDLK_LCTRL:    mv.moveKeys.set(Camera::CTRL);      break;
                case SDLK_LSHIFT:   mv.speedKey = true;                 break;
                default:                                                break;
                }

            } else if (Event.type == SDL_KEYUP) {

                switch (Event.key.keysym.sym)
                {
                case SDLK_w:        mv.moveKeys.reset(Camera::FWRD);        break;
                case SDLK_s:        mv.moveKeys.reset(Camera::BACK);        break;
                case SDLK_a:        mv.moveKeys.reset(Camera::LEFT);        break;
                case SDLK_d:        mv.moveKeys.reset(Camera::RGHT);        break;
                case SDLK_SPACE:    mv.moveKeys.reset(Camera::SPCE);        break;
                case SDLK_LCTRL:    mv.moveKeys.reset(Camera::CTRL);        break;
                case SDLK_LSHIFT:   mv.speedKey = false;                    break;
                default:                                                    break;
                }
            
            } else if (Event.type == SDL_QUIT)
            
                params[VIEW_PARAM::RUNNING] = 0;
            
            if (SDL_WINDOWEVENT_SIZE_CHANGED == Event.window.event) {

                WindowWidth = Event.window.data1;
                WindowHeight = Event.window.data2;
                glViewport(0, 0, WindowWidth, WindowHeight);
                scrRenderTarget.Recreate(WindowWidth, WindowHeight);
            }

            if (SDL_MOUSEMOTION == Event.type && params[VIEW_PARAM::MOUSELOOK]) {

                int xCenter = WindowWidth / 2;
                int YCenter = WindowHeight / 2;
                float xDiff = (float)Event.motion.xrel;// - xCenter;
                float yDiff = (float)Event.motion.yrel;// - YCenter;

                //SDL_WarpMouseInWindow(window, xCenter, YCenter);

                cam.ProcessMouse(xDiff, yDiff);
            }
        }

        cam.ProcessKeyboardAccel(mv, deltaTickTime);

        spaces.view = cam.GetViewMatrix();

        glBindBuffer(GL_UNIFORM_BUFFER, spaceTransUniform);
        glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(spaces.view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // Draw stuff to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, scrRenderTarget.multi_fb);
		glClearColor(0.05f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g.Draw();

        // now resolve multisampled buffer(s) into intermediate FBO
        glBindFramebuffer(GL_READ_FRAMEBUFFER, scrRenderTarget.multi_fb);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scrRenderTarget.postproc_fb);
        glBlitFramebuffer(0, 0, WindowWidth, WindowHeight, 0, 0, WindowWidth, WindowHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST);
        frameBufferShader.use();
        glBindVertexArray(scrRenderTarget.vaoScreenQuad);
        glBindTexture(GL_TEXTURE_2D, scrRenderTarget.postproc_texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(0);

        /*basic_shader.use();

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);*/

        SDL_GL_SwapWindow(Window);
    }
    return 0;
}