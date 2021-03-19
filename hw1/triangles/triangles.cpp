// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "../lib/shader.hpp"

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Triangles", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint varr1;
    glGenVertexArrays(1, &varr1);
    glBindVertexArray(varr1);
    GLuint shaders1 = LoadShaders( "vertex1.vertexshader", "fragment1.fragmentshader" );
    static const GLfloat vert1[] = { 
        -0.6f, 0.6f, 0.0f,
        -0.6f, -0.6f, 0.0f,
         0.5f,  0.5f, 1.0f,
    };
    GLuint buf1;
    glGenBuffers(1, &buf1);
    glBindBuffer(GL_ARRAY_BUFFER, buf1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert1), vert1, GL_STATIC_DRAW);

    GLuint varr2;
    glGenVertexArrays(1, &varr2);
    glBindVertexArray(varr2);
    static const GLfloat vert2[] = { 
        0.6f, 0.6f, 0.0f,
        0.6f, -0.6f, 0.0f,
        -0.5f,  0.5f, 1.0f,
    };
    GLuint shaders2 = LoadShaders( "vertex2.vertexshader", "fragment2.fragmentshader" );
    GLuint buf2;
    glGenBuffers(1, &buf2);
    glBindBuffer(GL_ARRAY_BUFFER, buf2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert2), vert2, GL_STATIC_DRAW);

    do{
        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram(shaders1);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buf1);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glUseProgram(shaders2);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buf2);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &buf1);
    glDeleteVertexArrays(1, &varr1);
    glDeleteProgram(shaders1);
    glDeleteBuffers(1, &buf2);
    glDeleteVertexArrays(1, &varr2);
    glDeleteProgram(shaders2);

    glfwTerminate();

    return 0;
}

