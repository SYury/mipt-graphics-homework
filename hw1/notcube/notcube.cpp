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
                                glm::vec3(3 * cos(time), 0, -1.5 + 3 * sin(time)),
                                glm::vec3(0,0, -1.5),
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

    window = glfwCreateWindow( 1920, 1080, "Not Cube", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    auto shaders = LoadShaders("vertex.vertexshader", "fragment.fragmentshader");

    auto matrix = glGetUniformLocation(shaders, "MVP");

    static const GLfloat vertices[] = {
        0.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -2.0f,
        -1.0f, -1.0f, -2.0f,
        0.0f, -1.0f, -1.0f,
        0.0f, 1.0f, -1.5f,
        1.0f, -1.0f, -2.0f,
        0.0f, -1.0f, -1.0f,
        0.0f, 1.0f, -1.5f,
        -1.0f, -1.0f, -2.0f,
        -1.0f, -1.0f, -2.0f,
        0.0f, 1.0f, -1.5f,
        1.0f, -1.0f, -2.0f
    };

    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };

    GLuint vbuf;
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint cbuf;
    glGenBuffers(1, &cbuf);
    glBindBuffer(GL_ARRAY_BUFFER, cbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    do{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaders);

        auto MVP = get_matrix(glfwGetTime());

        glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbuf);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cbuf);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, 12);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &vbuf);
    glDeleteBuffers(1, &cbuf);
    glDeleteProgram(shaders);
    glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();

    return 0;
}

