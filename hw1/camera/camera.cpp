// Include standard headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../lib/shader.hpp"

inline glm::mat4 get_matrix(double time){
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
                                glm::vec3(3 * cos(time), 1, 3 * sin(time)),
                                glm::vec3(0,0, 0),
                                glm::vec3(0,1,0)
                           );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    return MVP;
}

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
    window = glfwCreateWindow( 1920, 1080, "Camera", NULL, NULL);
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
    GLuint matr1 = glGetUniformLocation(shaders1, "MVP");
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
    GLuint matr2 = glGetUniformLocation(shaders2, "MVP");
    GLuint buf2;
    glGenBuffers(1, &buf2);
    glBindBuffer(GL_ARRAY_BUFFER, buf2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert2), vert2, GL_STATIC_DRAW);

    do{
        glClear( GL_COLOR_BUFFER_BIT );
        {
        glUseProgram(shaders1);
        auto MVP = get_matrix(glfwGetTime());
        glUniformMatrix4fv(matr1, 1, GL_FALSE, &MVP[0][0]);
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
        }
        {
        glUseProgram(shaders2);
        auto MVP = get_matrix(glfwGetTime());
        glUniformMatrix4fv(matr2, 1, GL_FALSE, &MVP[0][0]);
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
        }
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

