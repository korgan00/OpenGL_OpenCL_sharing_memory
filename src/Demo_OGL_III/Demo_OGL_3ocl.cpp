/*
 * Demo_3.cpp
 *
 *  Created on: 01/07/2013
 *      Author: Korgan
 *
 *  mingw32
 *  glew32
 *  opengl32
 *  SDL2main
 *  SDL2
 *
 */

#include "Demo_OGL_3ocl.h"

const char* Demo_OGL_3OCL::WIN_TITLE = "Titulo de la Ventana";
const GLfloat Demo_OGL_3OCL::VELOCITY = 10.0f;

const GLfloat Demo_OGL_3OCL::cube_positions[] = {
        -0.3f, -0.3f, -0.3f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f,
        -0.3f,  0.3f, -0.3f, 1.0f,
        -0.3f,  0.3f,  0.3f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f
};
const GLfloat Demo_OGL_3OCL::cube_colors[] = {
         1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  0.0f,  1.0f,
         1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  0.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  1.0f,  1.0f,
         0.0f,  1.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f
};
const GLushort Demo_OGL_3OCL::cube_indices[] = {
        0, 1, 2, 3, 6, 7, 4, 5, // First strip
        0xFFFF, // <<-- This is the restart index
        2, 6, 0, 4, 1, 5, 3, 7 // Second strip*/
};

const vmath::vec3 Demo_OGL_3OCL::X(1.0f, 0.0f, 0.0f);
const vmath::vec3 Demo_OGL_3OCL::Y(0.0f, 1.0f, 0.0f);
const vmath::vec3 Demo_OGL_3OCL::Z(0.0f, 0.0f, 1.0f);

Demo_OGL_3OCL::Demo_OGL_3OCL() : running(false), window(NULL), ctxt(NULL), info(),
            aspect(0), render_prog(0),
            render_model_matrix_loc(0), render_projection_matrix_loc(0),
            posX(0.0f), posY(0.0f), posZ(-2.0f), velocityXn(0.0f),
            velocityZn(0.0f), velocityYn(0.0f), velocityXp(0.0f),
            velocityZp(0.0f), velocityYp(0.0f),
            lastFrameTime((GLfloat)SDL_GetTicks() / 1000.0f),
            explode(false) {
	ebo[0] = vao[0] = vbo[0] = -1;
	vbo[1] = -1;

    for(int32_t i = 0; i < INST_LENGTH; i++){
        for(int32_t j = 0; j < INST_LENGTH; j++){
            for(int32_t k = 0; k < INST_LENGTH; k++){
                cube_rel_pos.push_back((GLfloat)i - INST_LENGTH / 2);
                cube_rel_pos.push_back((GLfloat)j - INST_LENGTH / 2);
                cube_rel_pos.push_back((GLfloat)-k);
                //cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*3+1] = (GLfloat)j-INST_LENGTH/2;
                //cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*3+2] = (GLfloat)-k;
                //cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*4+3] = 1;

            }
        }
    }

}

void Demo_OGL_3OCL::OnEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym){
                case SDLK_w:
                    velocityZp = VELOCITY;
                    break;
                case SDLK_a:
                    velocityXp = VELOCITY;
                    break;
                case SDLK_s:
                    velocityZn = VELOCITY;
                    break;
                case SDLK_d:
                    velocityXn = VELOCITY;
                    break;
                case SDLK_SPACE:
                    velocityYp = VELOCITY;
                    break;
                case SDLK_LCTRL:
                    velocityYn = VELOCITY;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event->key.keysym.sym){
                case SDLK_w:
                    velocityZp = 0.0f;
                    break;
                case SDLK_a:
                    velocityXp = 0.0f;
                    break;
                case SDLK_s:
                    velocityZn = 0.0f;
                    break;
                case SDLK_d:
                    velocityXn = 0.0f;
                    break;
                case SDLK_SPACE:
                    velocityYp = 0.0f;
                    break;
                case SDLK_e:
                    explode = true;
                    break;
                case SDLK_LCTRL:
                    velocityYn = 0.0f;
                    break;
                case SDLK_v:
                    std::cout << info.client_info() << std::endl << std::endl;
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
                default:
                    break;
            }
            break;
        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
    }
}
void Demo_OGL_3OCL::OnLoop() {
    GLfloat now = (GLfloat)SDL_GetTicks() / 1000.0f;
    dt = now - lastFrameTime;
    posX += (velocityXp - velocityXn) * dt;
    posZ += (velocityZp - velocityZn) * dt;
    posY += (velocityYn - velocityYp) * dt;
    lastFrameTime = now;
}

void Demo_OGL_3OCL::OnCleanup() {
    glUseProgram(0);
    glDeleteProgram(render_prog);
    glDeleteVertexArrays(1, vao);
    glDeleteBuffers(2, vbo);
    SDL_GL_DeleteContext(ctxt);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Demo_OGL_3OCL::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    window = SDL_CreateWindow(WIN_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIN_WIDTH, WIN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(!window)
        return false;

    SetupOpenGL();
    InitGLData();
    InitCLData();

    running = true;
    old_t = SDL_GetTicks() / 1000.0f;

    return true;
}

void Demo_OGL_3OCL::SetupOpenGL(){

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    ctxt = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(0);

    if (gl3wInit()) {
       std::cout << "Error al Inicializar GL3W" << std::endl;
    }

	if (!gl3wIsSupported(4,3)){
		std::cout << "Opengl 4.3 no es soportado" << std:: endl;
	}
}

int Demo_OGL_3OCL::OnExecute() {
    if (!Init())
        return -1;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event))
            Event(&event);

        Loop();
        Render();

        if(info.frame()){
            info.fps_ogl_ver_on_window(window, WIN_TITLE);
        }
    }
    Cleanup();
    return 0;
}

void Demo_OGL_3OCL::InitGLData(){
    printf("Initializing OGL Data\n");
    
    // Seleccionamos los shaders que queremos cargar
    ShaderInfo shaders[] = {
         { GL_VERTEX_SHADER, "../Shaders/Demo_OGL_III/geometry_instancingB.vs.glsl" },
         { GL_FRAGMENT_SHADER, "../Shaders/Demo_OGL_III/simple.fs.glsl" },
         { GL_NONE, NULL }
    };
    // Cargamos los shaders
    render_prog = LoadShaders( shaders );
    // Decimos a opengl que los utilice
    glUseProgram( render_prog );

    printf("\tShaders Loaded\n");

    // Seleccionamos la posicion dentro del shader de la matriz de modelado y la de proyeccion
    // Mas tarde vamos a usar esas posiciones para cambiar datos del cubo
    render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
    render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

    // Pedimos un buffer para el element buffer object
    glGenBuffers(1, ebo);
    // Le hacemos hueco diciendole el tipo de buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    // Lo rellenamos con los indices de los cubos
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // Pedimos un array de vertices
    glGenVertexArrays(1, vao);
    // Le hacemos hueco
    glBindVertexArray(vao[0]);

    // Pedimos un buffer para el vertex buffer object
    glGenBuffers(2, vbo);
    // Le hacemos hueco
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Le decimos que el hueco tiene que ser de tamaño "tamaño de cube positions"+"tamaño de cube colors"
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
    // Y como lo tenemos en dos arrays diferentes lo guardamos poco a poco
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions );
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

    // localizacion del atributo, numero de valores, tipo de valores,
    // normalizarlos, espacio entre valores, puntero al primer valor.
    // Por lo tanto decimos que nos pille los 4 primeros valores y
    // nos los meta en "posicion",
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    // Y apartir de cube positions que pille los colores.
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(cube_positions));
    // Activamos los dos arrays de atributos
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, cube_rel_pos.size() * sizeof(GLfloat), cube_rel_pos.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    printf("\tBuffers Loaded\n");

    //Seleccionamos el color de fondo
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Los triangulos que no se ven, ni los pinta (los triangulos que estan de espaldas)
    glEnable(GL_CULL_FACE);
    //Utiliza el z buffer
    glEnable(GL_DEPTH_TEST);
    //Funcion para el z-buffer
    glDepthFunc(GL_LESS);


    //lo que ocupa la parte en la que se dibuja.
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    aspect = float(WIN_HEIGHT) / float(WIN_WIDTH);

    printf("\tOGL Setup Complete\n");

    CheckErr();
    glFlush();
}

void Demo_OGL_3OCL::InitCLData() {
    explosion_pos = { 0.0f, 0.0f, -10.0f };

#if USE_OPENCL_KERNELS
    {
        printf("\nInitializing OCL Data\n");

        cl_device_id device_id = NULL;
        cl_uint ret_num_devices;
        cl_uint ret_num_platforms;

        cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
        cl_platform_id *platforms = NULL;
        platforms = (cl_platform_id*)malloc(ret_num_platforms * sizeof(cl_platform_id));

        ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
        ret = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

        printf("\tOCL Device selected\n");

        cl_context_properties properties[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
            CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[0], // OpenCL platform
            0
        };

        // Create a context using the supported devices
        cl_context context = clCreateContext(properties, 1, &device_id, NULL, NULL, &ret);

        printf("\tOCL-OGL Context entwined\n");

        // Create a reference cl_mem object from GL buffer object
        cl_vbo_pos = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, vbo[1], &ret);
        cl_mbo_vel = clCreateBuffer(context, CL_MEM_READ_WRITE, cube_rel_pos.size() * sizeof(cl_float), NULL, &ret);

        command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
        ret = clEnqueueAcquireGLObjects(command_queue, 1, &cl_vbo_pos, 0, 0, 0);

        //GLfloat *velocities = (GLfloat*)malloc(sizeof(GLfloat)*INSTANCES);
        cl_float zeroF = 0.0f;
        //ret = clEnqueueWriteBuffer(command_queue, cl_mbo_vel, CL_TRUE, 0, cube_rel_pos.size() * sizeof(GLfloat), NULL, 0, NULL, NULL);
        ret = clEnqueueFillBuffer(command_queue, cl_mbo_vel, &zeroF, sizeof(cl_float), 0, cube_rel_pos.size() * sizeof(cl_float), 0, NULL, NULL);
        //free(velocities);

        printf("\tOCL - OGL Buffer shared and filled\n");

        moveKernel = LoadKernel({ context, &device_id, "../Kernels/moveElements.cl", "moveElements" }, &ret);
        //velocityKernel = LoadKernel({ context, &device_id, "../Kernels/applyVelocity.cl", "applyVelocity" }, &ret);
    
        ret = clSetKernelArg(moveKernel.kernel, 0, sizeof(cl_mem), (void *)&cl_vbo_pos);
        ret = clSetKernelArg(moveKernel.kernel, 1, sizeof(cl_mem), (void *)&cl_mbo_vel);

        ret = clSetKernelArg(moveKernel.kernel, 2, sizeof(cl_float), (void *)&explosion_pos[0]);
        ret = clSetKernelArg(moveKernel.kernel, 3, sizeof(cl_float), (void *)&explosion_pos[1]);
        ret = clSetKernelArg(moveKernel.kernel, 4, sizeof(cl_float), (void *)&explosion_pos[2]);
        GLint instances = INSTANCES;
        ret = clSetKernelArg(moveKernel.kernel, 7, sizeof(GLint), (void *)&instances);

        clFlush(command_queue);
        printf("\tOCL - Kernel loaded and args setup\n");
        printf("\tOCL Setup complete\n");
    }
#else
    {
        for (int32_t i = 0; i < INST_LENGTH; i++) {
            for (int32_t j = 0; j < INST_LENGTH; j++) {
                for (int32_t k = 0; k < INST_LENGTH; k++) {
                    cube_vel.push_back(0.0f);
                    cube_vel.push_back(0.0f);
                    cube_vel.push_back(0.0f);
                }
            }
        }
    }
#endif
}

void Demo_OGL_3OCL::OnRender() {
    static int renders = 0;

#if USE_OPENCL_KERNELS
    {
        size_t globalItemSize = INSTANCES; // Process the entire lists
        size_t localItemSize = OPENCL_LOCAL_ITEM_SIZE; // Divide work items into groups of 64
        cl_int ret;

        ret = clEnqueueAcquireGLObjects(command_queue, 1, &cl_vbo_pos, 0, 0, 0);
        if (explode) {
            float amount = 50.0f;
            ret = clSetKernelArg(moveKernel.kernel, 5, sizeof(GLfloat), (void *)&amount);
            explode = false;
        } else {
            float amount = 0.0f;
            ret = clSetKernelArg(moveKernel.kernel, 5, sizeof(GLfloat), (void *)&amount);
        }
        if (renders++ < 10) printf("%d\n", CL_DEVICE_MAX_WORK_ITEM_SIZES);
        ret = clSetKernelArg(moveKernel.kernel, 6, sizeof(GLfloat), (void *)&dt);
        ret = clEnqueueNDRangeKernel(command_queue, moveKernel.kernel, 1, NULL, &globalItemSize, NULL, 0, NULL, NULL);
        if (ret == -54) {
            printf("work groups: %f\n", (float)INSTANCES / (float)OPENCL_LOCAL_ITEM_SIZE);
        }
        ret = clEnqueueReleaseGLObjects(command_queue, 1, &cl_vbo_pos, 0, 0, 0);

        clFlush(command_queue);
    }
#else 
    {
        int j = 0;
        float amount = 0.0f;
        if (explode) {
            amount = 50.0f;
            explode = false;
        }
        for (int i = 0; i < INSTANCES; ++i) {
            j = i * 3;
            float velX = cube_vel[j];
            float velY = cube_vel[j + 1];
            float velZ = cube_vel[j + 2];
            float posX = cube_rel_pos[j];
            float posY = cube_rel_pos[j + 1];
            float posZ = cube_rel_pos[j + 2];

            // Do the operation
            if (amount != 0) {
                float xDist = posX - explosion_pos[0];
                float yDist = posY - explosion_pos[1];
                float zDist = posZ - explosion_pos[2];
                float invDist = 1 / (xDist * xDist + yDist * yDist + zDist * zDist);
                velX += invDist * xDist * amount;
                velY += invDist * yDist * amount;
                velZ += invDist * zDist * amount;
            }

            cube_rel_pos[j] = posX + (velX * dt);
            cube_rel_pos[j + 1] = posY + (velY * dt);
            cube_rel_pos[j + 2] = posZ + (velZ * dt);
            cube_vel[j] = velX * (1 - dt * 0.2);
            cube_vel[j + 1] = velY * (1 - dt * 0.2);
            cube_vel[j + 2] = velZ * (1 - dt * 0.2);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, cube_rel_pos.size() * sizeof(GLfloat), cube_rel_pos.data(), GL_STATIC_DRAW);
    }
#endif

    // Limpiamos el buffer de profundidad (se pone al valor por defecto)
    // y el de color (se pone al valor de glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Decimos que Shader usar
    glUseProgram(render_prog);

    // Calculamos la matriz modelo
    vmath::mat4 model_matrix(vmath::translate(posX, posY, posZ) *
                                vmath::rotate(lastFrameTime * 36.0f, Y) *
                                vmath::rotate(lastFrameTime * 72.0f, Z));

    // Calculamos la matriz de proyección mediante un frustum
    vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));

    // Posicion en el shader, cantidad de matrices, es traspuesta? , matriz a setear.
    // Guardamos las dos matrices en el shader para que dibuje.
    // Al ser uniform el valor, se aprovecha entre los shaders
    glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);
    // Activamos el vertex array Object
    glBindVertexArray( vao[0] );
    // Activamos el buffer de indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);


#if USE_PRIMITIVE_RESTART
    // When primitive restart is on, we can call one draw command
    // Le decimos que active el reinicio de primitivas,
    glEnable(GL_PRIMITIVE_RESTART);
    // Este es el valor del indice que se toma para el reinicio
    glPrimitiveRestartIndex(0xFFFF);
    // Dibujamos como triangle strip (aprovechamos los dos vertices
    // anteriores para dibujar el siguiente triangulo)
    // un total de 17 indices y de tipo unsigned short sin offset (NULL)
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL, INSTANCES);

    
#else
    //Dibujamos un strip y otro.
    // Without primitive restart, we need to call two draw commands
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL, INSTANCES);
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,
            (const GLvoid *)(9 * sizeof(GLushort)), INSTANCES);
#endif
    // Buffer swap
    glFlush();
    SDL_GL_SwapWindow(window);
}




void Demo_OGL_3OCL::CheckErr() {
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
        std::cerr << "Error: " <<err;
}








