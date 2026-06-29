#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
    // initialise glfw functions
    glfwInit();

    // sets information used by the next glfwCreateWindow call
    // first tell glfw which OpenGL version we're using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // tell glfw to use the core profile, only modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Vertices coordinates
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(std::sqrt(3)) / 3, 0.0f, // Lower left corner
        0.5f, -0.5f * float(std::sqrt(3)) / 3, 0.0f, // Lower right corner
        0.0f, 0.5f * float(std::sqrt(3)) * 2 / 3, 0.0f // Upper corner
    };

    // Actually create our window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Sets the current context for the calling thread to the specified window context
    // basically telling glfw to use the window we've created
    glfwMakeContextCurrent(window);

    // load required configurations for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // defines the viewport within the window
    glViewport(0, 0, 800, 800);


    // Create a vertex shader and point it to our hard coded source code above
    // immediately compile it to machine code so it can be used by the GPU
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Do the same for a fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create a shader program to attach our shaders to
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // link the shader program
    glLinkProgram(program);

    // cleanup the shaders as they are now part of the program and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // ordering of VAO and VBO is important, VAO must be done before VBO

    // create a vertex buffer object to pass our vertex data to the gpu
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // specify a new "clear color" to be used, then execute the clear command on the color buffer
    // using our new specified color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // we specified the clear color on the back buffer, now we need to swap the back and front buffers
    // to display the color
    glfwSwapBuffers(window);

    // Keeps the window open and program running until the window is closed
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // process all events (window appearing, resize etc)
        // without this, the window will appear unresponsive
        glfwPollEvents();
    }

    // clear up buffers and programs
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);


    // teardown glfw and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}