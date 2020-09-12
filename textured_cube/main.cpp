#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

#include "../shader.h"
#include "texture.hpp"

using namespace glm;
GLFWwindow *window;


int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Textured Cube", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
//    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
//    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("../textured_cube/cube.vs.glsl", "../textured_cube/cube.fs.glsl");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, -20), // Camera is at (4,3,-3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices


    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    // Load the texture using any two methods
    //GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    GLuint Texture = loadDDS("../textured_cube/uvtemplate.DDS");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // Two UV coordinatesfor each vertex. They were created with Blender.
    static const GLfloat g_uv_buffer_data[] = {
            1.0f - 0.000059f, 0.000004f,
            1.0f - 0.000103f, 0.336048f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 1.000023f, 0.000013f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.999958f, 0.336064f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.336024f, 0.671877f,
            1.0f - 0.667969f, 0.671889f,
            1.0f - 1.000023f, 0.000013f,
            1.0f - 0.668104f, 0.000013f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.000059f, 0.000004f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 0.336098f, 0.000071f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 0.336024f, 0.671877f,
            1.0f - 1.000004f, 0.671847f,
            1.0f - 0.999958f, 0.336064f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.668104f, 0.000013f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 0.667979f, 0.335851f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 0.668104f, 0.000013f,
            1.0f - 0.336098f, 0.000071f,
            1.0f - 0.000103f, 0.336048f,
            1.0f - 0.000004f, 0.671870f,
            1.0f - 0.336024f, 0.671877f,
            1.0f - 0.000103f, 0.336048f,
            1.0f - 0.336024f, 0.671877f,
            1.0f - 0.335973f, 0.335903f,
            1.0f - 0.667969f, 0.671889f,
            1.0f - 1.000004f, 0.671847f,
            1.0f - 0.667979f, 0.335851f,
    };


    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    int cc = 0;
    do {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        cc++;
        auto m = Model
                 * glm::rotate(glm::mat4(1.0f), glm::radians(cc * 2.0f), glm::vec3(0.0f, 1.0f, 1.0f))
                 //                 * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.5f, 0.0f))
                 * glm::rotate(glm::mat4(1.0f), glm::radians(-cc * 3.0f), glm::vec3(1.0f, 0.0f, 1.0f));

        glm::mat4 MVP = Projection * View * m; // Remember, matrix multiplication is the other way around
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size : U+V => 2
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}