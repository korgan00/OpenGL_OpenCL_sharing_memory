/*
 * Demo_3.h
 *
 *  Created on: 01/07/2013
 *      Author: Korgan
 */

#ifndef DEMO_OGL_IIIB_H_
#define DEMO_OGL_IIIB_H_
#include "../Utils/SDL_OGL_GL3W_Dev_GeneralInclude.h"
#include "../Utils/LoadKernels.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#endif


class Demo_OGL_3OCL {
private:
    bool running;
    SDL_Window* window;
    SDL_GLContext ctxt;

    static const uint32_t   WIN_HEIGHT = DEFAULT_WIN_HEIGHT;
    static const uint32_t   WIN_WIDTH  = DEFAULT_WIN_WIDTH;
    static const char*      WIN_TITLE; //px
    static const int32_t    INST_LENGTH = 50;
    static const int32_t    INSTANCES = INST_LENGTH*INST_LENGTH*INST_LENGTH;
    Info_Manager info;
    /***************************************************/
    /***************************************************/
    /***************************************************/


    float aspect;
    static const GLfloat cube_positions[];
    static const GLfloat cube_colors[];
    static const GLushort cube_indices[];
    std::vector<GLfloat> cube_rel_pos;

    static const vmath::vec3 X;
    static const vmath::vec3 Y;
    static const vmath::vec3 Z;
    static const GLfloat VELOCITY;
    GLuint ebo[1];
    GLuint vao[1];
    GLuint vbo[2];
    cl_mem cl_vbo_pos;
    cl_mem cl_mbo_vel;
    cl_command_queue command_queue;
    //GLuint rel_pos_buff[1];

    GLuint render_prog;
    GLuint render_model_matrix_loc;
    GLuint render_projection_matrix_loc;
    KernelObjects moveKernel;
    KernelObjects velocityKernel;

    GLfloat posX;
    GLfloat posY;
    GLfloat posZ;
    GLfloat velocityXn;
    GLfloat velocityZn;
    GLfloat velocityYn;
    GLfloat velocityXp;
    GLfloat velocityZp;
    GLfloat velocityYp;
    GLfloat lastFrameTime;
    GLboolean explode;
    float old_t;

public:

    Demo_OGL_3OCL();

    /*
     * GAME LOOP FUNCTIONS
     */

    int Execute(){ return OnExecute(); }
    bool Init(){ return OnInit(); }
    void Loop(){ return OnLoop(); }
    void Render(){ return OnRender(); }
    void Cleanup(){ return OnCleanup(); }
    void Event(SDL_Event* Event){ OnEvent(Event); }


    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event* Event);
    void OnLoop();
    void OnRender();
    void OnCleanup();

    /***************************************************/
    /***************************************************/
    /***************************************************/

    void SetupOpenGL();
    void InitData();
    void InitCLData();

    void CheckErr();
};


#endif /* DEMO_OGL_I_H_ */
