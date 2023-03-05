#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};


static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream shadersSources[2];
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            shadersSources[(int)type] << line << '\n';
        }
    }

    return { shadersSources[0].str(), shadersSources[1].str() };
}


static unsigned int CompileShader(unsigned int glType, const std::string& source)
{
    unsigned int id = glCreateShader(glType);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, errorMessages);
        std::cout << "Failed to compile " << (glType == GL_VERTEX_SHADER? "vertex": "fragment") << " shader" << std::endl;
        std::cout << errorMessages << std::endl;
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "A white triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error! " << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Give OpenGL the data (copy as bytes to GPU memory)
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Copy data
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
    // Define a vertex attribute (position)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // Enable this attribute
    glEnableVertexAttribArray(0);


    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Create shaders as strings from file
    ShaderProgramSources sources = ParseShader("res/shaders/Basic.shader");
    /*std::cout << "VERTEX:" << std::endl;
    std::cout << sources.VertexSource << std::endl;
    std::cout << "FRAGMENT:" << std::endl;
    std::cout << sources.FragmentSource<< std::endl;*/

    unsigned int shader = CreateShader(sources.VertexSource, sources.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Legacy OpenGL for test purpose
        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        // Modern OpenGL
        // Will draw the bound buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleaning
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}