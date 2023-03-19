#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


const char* glsl_version = "#version 130";

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "A white triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error! " << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray vArray;
        VertexBuffer vBuffer(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout vLayout;
        vLayout.Push<float>(2);
        vLayout.Push<float>(2);
        vArray.AddBuffer(vBuffer, vLayout);

        IndexBuffer iBuffer(indices, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 0.0f));

        glm::mat4 mvp = proj * view * model;

        // Create shaders as strings from file
        std::string filepath = "res/shaders/Basic.shader";
        Shader shader(filepath);

        std::string textureFilepath = "res/textures/wood.png";
        Texture texture(textureFilepath);
        // Remember that shader must be bound before setting uniforms
        shader.Bind();
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4f("u_MVP", mvp);

        Renderer renderer;

        float f = 0.1f;
        float rotationAngle = 0.05f;
        glm::vec3 rotation(0.0f, 1.0, 0.0f);
        float blendAlpha = 0.5f;
        float r = 0.0f, increment = 0.005f, currentTime = 5.0f, deltaTime = 0.005f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Set input variables uniforms in shader
            shader.Bind();
            shader.SetUniform1f("u_Color", r);
            shader.SetUniform1f("u_time", currentTime);
            shader.SetUniform1f("u_alpha", blendAlpha);


            r += increment;
            if (r > 1.0f || r < 0.0f)
                increment = -increment;
            currentTime += deltaTime;
            if (currentTime > 5.5 || currentTime < 4.5)
                deltaTime = -deltaTime;

            {
                model = glm::translate(model, glm::vec3(0.0f, deltaTime / 2.0f, 0.0f));
                model = glm::rotate(model, rotationAngle, rotation);
                mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(vArray, iBuffer, shader);
            }

            {
                ImGui::SliderFloat("Blend texture", &blendAlpha, 0.0f, 1.0f);
                ImGui::SliderFloat("Rotation speed", &rotationAngle, -0.1f, 0.1f);
                ImGui::SliderFloat3("Rotation", &rotation.x, 0.0f, 1.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}