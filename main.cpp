#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include <GL/glew.h>        // GLEW library
#include <glm/glm.hpp>     // GLFW library
#include <glm/gtx/transform.hpp>     // GLFW library
#include <glm/gtc/type_ptr.hpp>     // GLFW library
#include <vector>
#include "MeshGenerator.hpp"
#include <learnOpengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#pragma once
using namespace std; // Uses the standard namespace

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Assignment 3-5 Chris Richards"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    struct GLMesh {
        GLuint vao; // Vertex Array object
        GLuint vbos[2]; // Vertex Buffer object
        GLuint nIndices; // Number of vertices in mesh
        Mesh object; // Object from our MeshGenerator
    };

    // Vertex Shader Program including model, view and projection matrices
    const GLchar* VERTEX_SHADER_SOURCE = GLSL(
        440,
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec4 color;
        layout(location = 2) in vec2 aTexCoord;

        out vec4 vertexColor;
        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(position, 1.0f);
            vertexColor = color;
            TexCoord = aTexCoord;
        }
    );

    // Fragment shader program
    const GLchar* FRAGMENT_SHADER_SOURCE = GLSL(
        440,
        in vec4 vertexColor;
        in vec2 TexCoord;

        out vec4 fragmentColor;

        uniform sampler2D texSample;

        void main()
        {
            fragmentColor = texture(texSample, TexCoord) * vec4(vertexColor);
        }
    );

    GLFWwindow* gWindow = nullptr;
    GLMesh gMesh1;
    GLMesh gMesh2;
    GLuint gProgramId;

    // Camera
    Camera gCamera(glm::vec3(0.0f, 5.0f, 20.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;
    float cameraSpeed = 1.0f;

    // Timing variables
    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;

    // Projections
    glm::mat4 perspective = glm::perspective(glm::radians(gCamera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 ortho = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    glm::mat4 currentProjection;
    double lastProjectionChange = glfwGetTime();
}


/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char*[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
unsigned int ULoadTexture(const char* texFile);


// main function. Entry point to the OpenGL program
int main(int argc, char* argv[])
{

    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE, gProgramId))
        return EXIT_FAILURE;

    // Create square base of candle holder
    //gMesh1.object = generatePyramid(2.0f, 2.0f, true);
    gMesh1.object = generatePlane(10.0f, 10.0f, Position(0.0f, 0.0f, 0.0f), true);

    gMesh1.object.texture = ULoadTexture("bricks.jpg");
    cout << gMesh1.object.texture << endl;
    UCreateMesh(gMesh1);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        // input
        // -----
        UProcessInput(gWindow);

        URender();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
    }

    UDestroyMesh(gMesh1);
    
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    //gCamera.Position += BACKWARD * 10.0;
    currentProjection = perspective;
    // GLFW: initialize and configure (specify desired OpenGL version)
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.Position += gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.Position -= gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (glfwGetTime() - lastProjectionChange) > 0.5)
    {
        currentProjection = currentProjection == perspective ? ortho : perspective;
        lastProjectionChange = glfwGetTime();
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId) {
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}

void UDestroyShaderProgram(GLuint programId) {
    glDeleteProgram(programId);
}

void UCreateMesh(GLMesh& mesh)
{
    const GLuint FLOATS_PER_VERTEX = 3;
    const GLuint FLOATS_PER_COLOR = 4;
    const GLuint FLOATS_PER_TEX_COORD = 2;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(2, mesh.vbos); // Creates 2 buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, mesh.object.vertices.size() * sizeof(float), &mesh.object.vertices.front(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = mesh.object.indices.size() * sizeof(short);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.nIndices, &mesh.object.indices.front(), GL_STATIC_DRAW);

    // Stride of each vertex coordinate in array
    GLint stride = sizeof(float) * (FLOATS_PER_VERTEX + FLOATS_PER_COLOR + FLOATS_PER_TEX_COORD);

    // Send vertex coordinates to GPU
    glVertexAttribPointer(0, FLOATS_PER_VERTEX, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(GLfloat) * FLOATS_PER_VERTEX));
    glEnableVertexAttribArray(1);

    if (mesh.object.isTextured) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * (FLOATS_PER_VERTEX + FLOATS_PER_COLOR)));
        glEnableVertexAttribArray(2);
    }

}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

void URender() {
    
    glEnable(GL_DEPTH_TEST);

    // Clear the background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Object rotation
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

    // Rotate object
    glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 0.5f, 0.0f));

    // Place object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // Model Matrix
    glm::mat4 model = translation * rotation * scale;

    // Transform Camera
    glm::mat4 view = gCamera.GetViewMatrix();

    // Create projection
    glm::mat4 projection = currentProjection;

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieve and pass transforms to shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projectionLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gMesh1.object.texture);
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh1.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh1.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle meshes
    
    // Deactivate the VAO
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Bind camera movement to mouse cursor.
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (gFirstMouse) {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos;

    gLastX = xpos;
    gLastY = ypos;
    //cout << "Mouse(x:y) = " << xpos << ":" << ypos << endl;
    gCamera.ProcessMouseMovement(xoffset, yoffset);
}

void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    gCamera.MovementSpeed = max(0.2f, min(20.0f, gCamera.MovementSpeed + (float)yoffset));
}

void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {
            cout << "Left mouse button pressed" << endl;
        }
        break;
    }
}

unsigned int ULoadTexture(const char* texFile) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texFile, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    return texture;
}