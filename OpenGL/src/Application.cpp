#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "Application.h"

#include "scene/Scene.h"
#include "scene/MenuScene.h"
#include "scene/Texture2DScene.h"
#include "scene/TriangleScene.h"

void main(void)
{
    GLFWwindow* window = init();

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    unsigned int indicies[] = {
        0, 1, 2
    };
    
    scene::Scene* currentScene = nullptr;

    scene::MenuScene* MenuScene = new scene::MenuScene(currentScene);
    currentScene = MenuScene;

    MenuScene->RegisterScene<scene::TriangleScene>("Triangle");
    MenuScene->RegisterScene<scene::Texture2DScene>("Textures");

    MenuScene->SetScene("Textures");

    Renderer renderer;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentScene) {
            currentScene->OnUpdate(0.0f);
            currentScene->OnRender();

            ImGui::Begin("Application");
            if (currentScene != MenuScene) {

                if (ImGui::Button("Back to Scene Menu")) {
                    delete currentScene;
                    currentScene = MenuScene;
                }

            }
            currentScene->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

GLFWwindow* init() {
    GLFWwindow* window;
    if (!glfwInit())
        return 0;

    window = glfwCreateWindow(960, 540, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

ShaderProgramSource ParseShaderProgram(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}